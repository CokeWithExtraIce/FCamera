#pragma once

#include <QString>
#include <QImage>

/*
Context 특정 작업을 수행하는데 필요한 설정, 상태, 관련 정보를 저장한 객체
AVFormatContext 미디어 컨테이너를 열고 스트림/데이터 전체를 관리하기 위한 context
AVStream 컨테이너 내부에 위치한 하나의 스트림(영상, 음성 등)
AVCodecParameters 해당 스트림의 코덱 종류와 디코딩에 필요한 매개변수 정보를 담는 객체
AVCodec 이 코덱을 처리할 수 있는 디코더의 종류/정보
AVCodecContext 특정 디코더를 실행하기 위한 상태
    디코딩 관련 설정, 영상 크기, 디코더 실행에 필요한 정보 등 저장
AVPacket FFmpeg가 input에서 읽어온 압축된 미디어 데이터 (압축된 h.264 영상 데이터)
AVFrame AVPacket등 압축된 데이터를 decoder로 처리한 결과로 출력하는 미디어 데이터(디코딩된 영상 프레임)
*/
// 전방 선언
struct AVFormatContext;
struct AVCodecContext;
struct AVPacket;
struct AVFrame;

class FFmpegVideoDecoder
{
public:
    FFmpegVideoDecoder();
    ~FFmpegVideoDecoder();

    // 영상 파일을 열고 VideoStream 및 Decoder 준비.
    bool open(const QString& filename);

    // 첫 번째 VideoFrame 디코딩.
    QImage decodeNextFrame();

private:
    // 컨테이너에서 VideoStream 탐색.
    bool findVideoStream();

    // VideoStream에 맞는 FFmpegDecoder를 준비.
    bool openDecoder();

    // FFmpeg의 AVFrame을 Qt의 QImage로 변환.
    QImage convertFrameToImage(const AVFrame* frame);

private:
    AVFormatContext* m_formatContext = nullptr;
    AVCodecContext* m_codecContext = nullptr;
    AVPacket* m_packet = nullptr;
    AVFrame* m_frame = nullptr;

    int m_videoStreamIndex = -1;
};