#include "config/local_config.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // 1. 영상 파일 열기

    const char* filename = SAMPLE_VIDEO_PATH;

    AVFormatContext* formatContext = nullptr;

    int ret = avformat_open_input(
        &formatContext,
        filename,
        nullptr,
        nullptr
        );

    if (ret < 0) {
        qDebug() << "Failed to open video:" << filename;
        return -1;
    }

    qDebug() << "Video opened successfully:" << filename;


    // 2. 영상 파일의 스트림 정보 확인

    ret = avformat_find_stream_info(formatContext, nullptr);

    if (ret < 0) {
        qDebug() << "Failed to find stream information.";
        avformat_close_input(&formatContext);
        return -1;
    }

    qDebug() << "Stream information found.";
    qDebug() << "Number of streams:" << formatContext->nb_streams;


    // 3. Video Stream 찾기

    int videoStreamIndex = -1;

    for (unsigned int i = 0; i < formatContext->nb_streams; ++i) {
        AVStream* stream = formatContext->streams[i];

        if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = static_cast<int>(i);
            break;
        }
    }

    if (videoStreamIndex == -1) {
        qDebug() << "No video stream found.";
        avformat_close_input(&formatContext);
        return -1;
    }

    qDebug() << "Video stream found:" << videoStreamIndex;


    // 4. Video Stream의 코덱 정보 확인

    AVStream* videoStream = formatContext->streams[videoStreamIndex];

    AVCodecParameters* codecParameters = videoStream->codecpar;

    qDebug() << "Video codec ID:" << codecParameters->codec_id;


    // 5. Video Codec에 해당하는 Decoder 찾기

    const AVCodec* codec = avcodec_find_decoder(
        codecParameters->codec_id
        );

    if (codec == nullptr) {
        qDebug() << "Failed to find video decoder.";
        avformat_close_input(&formatContext);
        return -1;
    }

    qDebug() << "Video decoder found:" << codec->name;


    // 6. Decoder Context 생성

    AVCodecContext* codecContext = avcodec_alloc_context3(codec);

    if (codecContext == nullptr) {
        qDebug() << "Failed to allocate codec context.";
        avformat_close_input(&formatContext);
        return -1;
    }


    // 7. Stream의 Codec Parameters를 Decoder Context에 복사

    ret = avcodec_parameters_to_context(
        codecContext,
        codecParameters
        );

    if (ret < 0) {
        qDebug() << "Failed to copy codec parameters.";
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return -1;
    }


    // 8. Decoder 열기

    ret = avcodec_open2(
        codecContext,
        codec,
        nullptr
        );

    if (ret < 0) {
        qDebug() << "Failed to open video decoder.";
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return -1;
    }

    qDebug() << "Video decoder opened successfully.";


    // 9. Packet과 Frame 생성

    AVPacket* packet = av_packet_alloc();
    AVFrame* frame = av_frame_alloc();

    if (packet == nullptr || frame == nullptr) {
        qDebug() << "Failed to allocate packet or frame.";
        av_packet_free(&packet);
        av_frame_free(&frame);
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return -1;
    }


    // 10. 영상 Packet 읽기

    bool decodingFinished = false;

    while (av_read_frame(formatContext, packet) >= 0) {

        // Video Stream의 Packet만 처리

        if (packet->stream_index == videoStreamIndex) {

            ret = avcodec_send_packet(codecContext, packet);

            if (ret < 0) {
                qDebug() << "Failed to send packet to decoder.";
                break;
            }


            // 11. Packet에서 디코딩된 Frame 가져오기

            while (ret >= 0) {
                ret = avcodec_receive_frame(codecContext, frame);

                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                    break;
                }

                if (ret < 0) {
                    qDebug() << "Failed to receive frame from decoder.";
                    break;
                }

                qDebug() << "Decoded frame:"
                         << "width =" << frame->width
                         << "height =" << frame->height
                         << "format =" << frame->format;

                // 첫 번째 Frame을 확인했으므로 디코딩 종료

                decodingFinished = true;
                break;
            }
        }

        av_packet_unref(packet);

        if (decodingFinished) {
            break;
        }
    }


    // 12. FFmpeg 리소스 정리

    av_packet_free(&packet);
    av_frame_free(&frame);
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);


    // 13. QML 애플리케이션 실행

    QQmlApplicationEngine engine;

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() {
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection
        );

    engine.loadFromModule("FCamera", "Main");

    return app.exec();
}