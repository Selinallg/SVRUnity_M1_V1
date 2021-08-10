#ifndef NXR_SENSOR_TYPES_H
#define NXR_SENSOR_TYPES_H

#include "../NxrClient/inc/QVRServicePlugin.h"
#include "../NxrClient/inc/QVRCameraClient.h"
#include "../NxrClient/inc/QVRServiceClient.h"

namespace nxr_client{
    enum SxrResult
    {
        // Return code for success
        SXR_ERROR_NONE = 0,

        // Return code for cases that don't fall into defined error codes
        SXR_ERROR_UNKNOWN = 1,

        // Return code to indicate that SXR isn't supported, or the necessary functionality in the system drivers
        // or VR service isn't supported.
        SXR_ERROR_UNSUPPORTED,

        // Return code for calls to functions made without first calling sxrInitialize
        SXR_ERROR_VRMODE_NOT_INITIALIZED,

        // Return code for calls made into functions that require VR mode to be in a started state
        SXR_ERROR_VRMODE_NOT_STARTED,

        // Return code for calls made into functions that require VR mode to be in a stopped state
        SXR_ERROR_VRMODE_NOT_STOPPED,

        // Return code for calls made into functions where the service fails or isn't accessible
        SXR_ERROR_QVR_SERVICE_UNAVAILABLE,

        // Error for any failures in JNI/Java calls made through the API
        SXR_ERROR_JAVA_ERROR,
    };

    struct NxrPredictedData{
        float rotation[4]; // x,y,z,w
        float translation[3];// x,y,z
    };

    struct NxrStreamData{
        unsigned char data[4096000];
        unsigned char left[2048000];
        unsigned char right[2048000];
        uint16_t type;
        uint16_t format;
        uint16_t width;
        uint16_t height;
        uint32_t size;
        uint32_t leftSize;
        uint32_t rightSize;
        uint64_t timestamp;
    };

    enum StreamType {
        STREAM_NONE = 0,
        STREAM_EYETRACKING,
        STREAM_RGB,
        STREAM_TOF,
        STREAM_FISHEYE,
        STREAM_THERMAL,
    };

    struct NxrSlamData {
        float data[32];
        int length;
        long long int dataTimeStampMs;
        int source;
    };

    typedef void (*fn_stream_callback)(StreamType type,NxrStreamData *);

    
};//namespace nxr_client

#endif //NXR_SENSOR_TYPES_H