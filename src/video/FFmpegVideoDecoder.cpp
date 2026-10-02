#include "FFmpegVideoDecoder.h"

#include <QDebug>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

FFmpegVideoDecoder::FFmpegVideoDecoder()
{
}

FFmpegVideoDecoder::~FFmpegVideoDecoder()
{
    // FFmpeg에서 할당한 리소스 해제
    av_frame_free(&m_frame);
    av_packet_free(&m_packet);
    avcodec_free_context(&m_codecContext);
    avformat_close_input(&m_formatContext);
}

bool FFmpegVideoDecoder::open(const QString& filename)
{
    const QByteArray path = filename.toLocal8Bit();

    // 미디어 컨테이너 열기 및 FormatContext 생성
    int ret = avformat_open_input(
        &m_formatContext,
        path.constData(),
        nullptr,
        nullptr
        );

    if (ret < 0) {
        qDebug() << "Failed to open video:" << filename;
        return false;
    }

    qDebug() << "Video opened successfully:" << filename;

    // 컨테이너의 Stream 정보 탐색
    ret = avformat_find_stream_info(
        m_formatContext,
        nullptr
        );

    if (ret < 0) {
        qDebug() << "Failed to find stream information.";
        return false;
    }

    qDebug() << "Stream information found.";
    qDebug() << "Number of streams:"
             << m_formatContext->nb_streams;

    // VideoStream 탐색
    if (!findVideoStream()) {
        return false;
    }

    // VideoStream에 맞는 Decoder 준비
    if (!openDecoder()) {
        return false;
    }

    // 압축된 미디어 데이터를 저장할 Packet과
    // 디코딩된 데이터를 저장할 Frame 생성
    m_packet = av_packet_alloc();
    m_frame = av_frame_alloc();

    if (m_packet == nullptr || m_frame == nullptr) {
        qDebug() << "Failed to allocate packet or frame.";
        return false;
    }

    return true;
}


bool FFmpegVideoDecoder::findVideoStream()
{
    // 컨테이너의 Stream을 순회하면서 VideoStream 탐색
    for (unsigned int i = 0;
         i < m_formatContext->nb_streams;
         ++i) {

        AVStream* stream = m_formatContext->streams[i];

        // Stream의 MediaType이 Video인지 확인
        if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {//ffmpeg 에서 정의된 enum

            // VideoStream의 인덱스 저장
            m_videoStreamIndex = static_cast<int>(i);//unsigned int -> int (-1 사용 구문 있음)

            qDebug() << "Video stream found:"
                     << m_videoStreamIndex;

            return true;
        }
    }

    qDebug() << "No video stream found.";
    return false;
}


bool FFmpegVideoDecoder::openDecoder()
{
    // 탐색한 VideoStream 가져오기
    AVStream* videoStream =
        m_formatContext->streams[m_videoStreamIndex];

    // VideoStream의 CodecParameters 가져오기
    AVCodecParameters* codecParameters =
        videoStream->codecpar;

    qDebug() << "Video codec ID:"
             << codecParameters->codec_id;

    // Codec ID에 맞는 Decoder 탐색
    const AVCodec* codec =
        avcodec_find_decoder(codecParameters->codec_id);

    if (codec == nullptr) {
        qDebug() << "Failed to find video decoder.";
        return false;
    }

    qDebug() << "Video decoder found:"
             << codec->name;

    // Decoder 실행을 위한 CodecContext 생성
    m_codecContext = avcodec_alloc_context3(codec);

    if (m_codecContext == nullptr) {
        qDebug() << "Failed to allocate codec context.";
        return false;
    }

    // Stream의 CodecParameters를 DecoderContext에 복사
    int ret = avcodec_parameters_to_context(
        m_codecContext,
        codecParameters
        );

    if (ret < 0) {
        qDebug() << "Failed to copy codec parameters.";
        return false;
    }

    // Decoder 실행 준비
    ret = avcodec_open2(
        m_codecContext,
        codec,
        nullptr
        );

    if (ret < 0) {
        qDebug() << "Failed to open video decoder.";
        return false;
    }

    qDebug() << "Video decoder opened successfully.";

    return true;
}


QImage FFmpegVideoDecoder::decodeNextFrame()
{
    // Decoder가 준비되지 않은 경우 종료
    if (m_formatContext == nullptr ||
        m_codecContext == nullptr ||
        m_packet == nullptr ||
        m_frame == nullptr) {
        return {};
    }

    // 미디어 입력에서 Packet 하나씩 읽기.
    while (av_read_frame(m_formatContext, m_packet) >= 0) {

        // 읽은 Packet이 VideoStream의 데이터인지 확인
        if (m_packet->stream_index == m_videoStreamIndex) {

            //VideoPacket을 Decoder에 전달
            int ffmpegResult = avcodec_send_packet(
                m_codecContext,
                m_packet
                );

            //다음 Packet을 위해 현재 Packet 참조 해제
            av_packet_unref(m_packet);

            if (ffmpegResult < 0) {
                qDebug() << "Failed to send packet to decoder.";
                return {};
            }

            // Decoder로 처리한 Frame을 가져오기.
            while (ffmpegResult >= 0) {

                ffmpegResult = avcodec_receive_frame(
                    m_codecContext,
                    m_frame
                    );

                // 아직 Frame을 받을 수 없는 경우
                if (ffmpegResult == AVERROR(EAGAIN) || //디코더가 출력가능한 완성된 Frame이 없음(새로운 Packet 입력 필요)
                    ffmpegResult == AVERROR_EOF) { //더이상 디코딩할 입력이 없다
                    break;
                }

                if (ffmpegResult < 0) {
                    qDebug()
                    << "Failed to receive frame from decoder.";

                    return {};
                }

                // qDebug() << "Decoded frame:"
                //          << "width =" << m_frame->width
                //          << "height =" << m_frame->height
                //          << "format =" << m_frame->format;

                // VideoFrame 디코딩 성공
                // FFmpeg의 AVFrame을 Qt에서 사용할 수 있는 QImage로 변환
                QImage image = convertFrameToImage(m_frame);
                if (image.isNull()){
                    qDebug() << "Failed to conver frame to QImage";
                    return {};
                }
                return image;
            }
        } else {
            //VideoStream이 아닌 Packet은 사용하지 않음
            av_packet_unref(m_packet);
        }
    }

    return {};
}

QImage FFmpegVideoDecoder::convertFrameToImage(
    const AVFrame* frame)
{
    if (frame == nullptr) {
        return {};
    }

    // Qt에서 사용할 RGB 형식의 Image 생성
    QImage image(
        frame->width,
        frame->height,
        QImage::Format_RGB888
        );

    if (image.isNull()) {
        qDebug() << "Failed to create QImage.";
        return {};
    }


    // swscale : FFmpeg에서 영상의 픽셀 데이터를 변환하는 라이브러리, sw + scale
    // AVFrame의 픽셀 표현 방식은 YUV, QImage는 RGB
    // 기호 Y(밝기), U(blue 방향 색차), V(red 방향 색차)

    // FFmpeg Frame의 YUV기반 PixelFormat을
    // Qt에서 사용할 RGB24 형식으로 변환하기 위한 Context 생성
    SwsContext* swsContext = sws_getContext(
        frame->width,
        frame->height,
        static_cast<AVPixelFormat>(frame->format),

        frame->width,
        frame->height,
        AV_PIX_FMT_RGB24,

        SWS_BILINEAR,
        nullptr,
        nullptr,
        nullptr
        );

    if (swsContext == nullptr) {
        qDebug() << "Failed to create SwsContext.";
        return {};
    }

    // 변환된 Pixel 데이터를 저장할 QImage의 메모리 주소
    uint8_t* destinationData[4] = {
        image.bits(),
        nullptr,
        nullptr,
        nullptr
    };

    // QImage의 한 줄당 Byte 수
    int destinationLinesize[4] = {
        static_cast<int>(image.bytesPerLine()),
        0,
        0,
        0
    };

    // FFmpeg Frame의 PixelFormat을 RGB24로 변환
    sws_scale(
        swsContext,
        frame->data,
        frame->linesize,
        0,
        frame->height,
        destinationData,
        destinationLinesize
        );

    // PixelFormat 변환에 사용한 Context 해제
    sws_freeContext(swsContext);

    return image;
}