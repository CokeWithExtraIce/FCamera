#pragma once

#include <QImage>

// Device에서 데이터를 가져오는 방법을 추상화한 인터페이스.

/*
    example:
    FileInput   -> 영상 파일에서 Frame 획득
    WebcamInput -> 웹캠에서 Frame 획득
    RTSPInput   -> RTSP 카메라에서 Frame 획득
*/

class DeviceInput
{
public:
    virtual ~DeviceInput() = default;

    // 입력 장치를 준비한다.
    virtual bool open() = 0;

    // 다음 Frame을 가져옵니다.
    // 가져올 Frame이 없거나 오류가 발생하면 null QImage를 반환합니다.
    virtual QImage readFrame() = 0;
};