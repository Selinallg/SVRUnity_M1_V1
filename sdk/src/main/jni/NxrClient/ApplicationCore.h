//
// Created by xiaoyang on 2021/5/5.
//

#ifndef SVRUNITY_M1_V1_APPLICATIONCORE_H
#define SVRUNITY_M1_V1_APPLICATIONCORE_H
#include <stdlib.h>
#include <mutex>
#include <thread>
#include "utils/VeraSignal.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)    (sizeof(a)/sizeof(a[0]))   //xiaoyang.4.0.4
#endif
namespace Xiao{
    //--------------------------------------------xiaoyang
    struct QVRCameraFrameData {
        uint8_t *dataArray = nullptr;
        uint32_t frameIndex = 0;
        uint64_t exposureNano = 0;
    };

    struct SvrAppContext{
        //----------------------------------------xiaoyang
        //                   camera
        //----------------------------------------
        int                        qvrCameraClientHandle=0;
        int                        qvrDeviceClientHandle=0;
        qvrcamera_client_helper_t *qvrCameraClient = nullptr;
        qvrcamera_device_helper_t *qvrDeviceClient = nullptr;
        int qvrCameraApiVersion = 0;
        uint16_t qvrCameraWidth = 0;
        uint16_t qvrCameraHeight = 0;
        std::mutex qvrCameraMutex;
        std::thread qvrCameraThread;
        Vera::VeraSignal qvrCameraStartSignal{false};
        Vera::VeraSignal qvrCameraTerminalSignal{false};
        int qvrNewBufferIdx = -1;
        int qvrAvailableBufferIdx = -1;
    };

    static QVRCAMERA_FRAME_FORMAT CamFrameFormatFromString(const char* str)
    {
        const char* table[QVRCAMERA_FRAME_FORMAT_LAST] = {
                "unknown",                            // QVRCAMERA_FRAME_FORMAT_UNKNOWN
                QVR_CAMDEVICE_FRAME_FORMAT_Y8,        // QVRCAMERA_FRAME_FORMAT_Y8
                QVR_CAMDEVICE_FRAME_FORMAT_YUV420,    // QVRCAMERA_FRAME_FORMAT_YUV420
                QVR_CAMDEVICE_FORMAT_RAW10,           // QVRCAMERA_FRAME_FORMAT_RAW10_MONO
                "depth16",                            // QVRCAMERA_FRAME_FORMAT_DEPTH16
                "rawdepth",                           // QVRCAMERA_FRAME_FORMAT_RAW_DEPTH
                QVR_CAMDEVICE_FORMAT_RAW16            // QVRCAMERA_FRAME_FORMAT_RAW16_MONO
        };
        for (uint32_t i=0; i<ARRAY_SIZE(table); i++)
        {
            if (0 == strcmp(table[i], str))
            {
                return (QVRCAMERA_FRAME_FORMAT) i;
            }
        }
        return QVRCAMERA_FRAME_FORMAT_UNKNOWN;
    }

    static const char* CamFrameFormatToString(QVRCAMERA_FRAME_FORMAT format)
    {
        switch (format)
        {
            case QVRCAMERA_FRAME_FORMAT_Y8:         return QVR_CAMDEVICE_FRAME_FORMAT_Y8; break;
            case QVRCAMERA_FRAME_FORMAT_YUV420:     return QVR_CAMDEVICE_FRAME_FORMAT_YUV420; break;
            case QVRCAMERA_FRAME_FORMAT_RAW10_MONO: return QVR_CAMDEVICE_FORMAT_RAW10; break;
            case QVRCAMERA_FRAME_FORMAT_DEPTH16:    return "depth16"; break;
            case QVRCAMERA_FRAME_FORMAT_RAW_DEPTH:  return "rawdepth"; break;
            case QVRCAMERA_FRAME_FORMAT_RAW16_MONO: return QVR_CAMDEVICE_FORMAT_RAW16; break;
            case QVRCAMERA_FRAME_FORMAT_UNKNOWN:
            default:                                return "unknown"; break;
        }
    }


}

#endif //SVRUNITY_M1_V1_APPLICATIONCORE_H
