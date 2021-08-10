#ifndef _SVR_UNITY_H_
#define _SVR_UNITY_H_

#include <jni.h>
#include <sys/syscall.h>
#include <memory.h>
#include <unistd.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>

#include <android/native_window_jni.h>
#include <android/log.h>
#include <string>
#include <GLES2/gl2ext.h>

#include "IUnityInterface.h"
#include "IUnityGraphics.h"
#include "svrUnity.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/transform.hpp"
#include "NxrClient/NxrMoudle.h"
#include "NxrClient/INxrSensorClient.h"
#include "NxrClient/ApplicationCore.h"
#include "NxrClient/utils/VeraSignal.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>
namespace {
    using namespace  std;
    using namespace  Xiao;
    using namespace  nxr_client;
    JavaVM*				javaVm;
    NxrMoudle                                        nXrInterface;
    android::INxrSensorClient *                      nXrClient;
    const int                                        imageWidth=1280;
    const int                                        imageHeight=400;
    bool                                             outBUdate = true;
    uint32_t                                         outCurrFrameIndex = 0;
    uint64_t                                         outFrameExposureNano = 0;
    //-------------------------双眼数据
    uint8_t*                                         outFrameData;
    float                                            outTRDataArray[7];
    //----------------------------------------xiaoyang
    //                   camera
    //----------------------------------------
    bool                                             gUseQVRCamera = true;
    SvrAppContext*                                   gAppContext = NULL;
    constexpr int                                    QVR_CAMERA_BUFFER_NUM = 2;
    int                                              gQVRFrameIndexArray[QVR_CAMERA_BUFFER_NUM] = {-1};
    QVRCameraFrameData                               gQVRCameraFrameDataArray[QVR_CAMERA_BUFFER_NUM];

}

JNIEnv* GetJNIEnv(JavaVM* javaVM)
{
    JNIEnv* jniEnv;
    javaVM->AttachCurrentThread(&jniEnv, NULL);
    return jniEnv;
}

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    LogDebug("JNI_OnLoad() called!");
    JNIEnv* javaEnv;
    if (vm->GetEnv(reinterpret_cast<void**>(&javaEnv), JNI_VERSION_1_2) != JNI_OK)
    {
        return -1;
    }

    // Get jclass with env->FindClass.
    // Register methods with env->RegisterNatives.
    javaVm = vm;
    return JNI_VERSION_1_2;
}

inline std::string GetStringFromJNIString(JNIEnv *env, jstring string){
    const char *buf = env->GetStringUTFChars(string, 0);
    std::string ret = buf;
    env->ReleaseStringUTFChars(string, buf);

    return ret;
}

void fetchingQVRCameraFrame() {
    LogSvr("fetchingQVRCameraFrame start");
    bool bCameraOpen = false;
    QVRCAMERA_CAMERA_STATUS cameraStatus = QVRCAMERA_CAMERA_ERROR;
    int currQVRFrameIndex = -1;
    qvrcamera_frame_t currQVRFrame;
    std::chrono::high_resolution_clock::time_point lastFetchNano;
    constexpr long INTERVAL_NANO = 1e9 / 60;
    int res = 0;
    bool bGoodFrame = true;
    while (!gAppContext->qvrCameraTerminalSignal.isSignaled()) {
        if (!gAppContext->qvrCameraStartSignal.isSignaled()) {
            LogSvr("fetchingQVRCameraFrame before sleep");
            gAppContext->qvrCameraStartSignal.waitSignal(Vera::VeraSignal::SIGNAL_TIMEOUT_INFINITE);
            LogSvr("fetchingQVRCameraFrame after sleep");
        }
        auto startNano = std::chrono::high_resolution_clock::now();
        auto deltaNanos = std::chrono::duration_cast<std::chrono::nanoseconds>(
                startNano - lastFetchNano).count();
        if (deltaNanos > INTERVAL_NANO)
        {
            //usleep(16000);
            res = nXrClient->NXRCameraDevice_GetCurrentFrameNumber(&currQVRFrameIndex);
            bGoodFrame = (res == QVR_CAM_SUCCESS) && (currQVRFrameIndex >= 0);
            LogSvr("fetchingQVRCameraFrame-1-QVRCameraDevice_GetCurrentFrameNumber:%d %d %d",res,currQVRFrameIndex,bGoodFrame);
            if (bGoodFrame) {
                bGoodFrame = gAppContext->qvrNewBufferIdx < 0 ||
                             (gAppContext->qvrNewBufferIdx >= 0 &&
                              gQVRFrameIndexArray[gAppContext->qvrNewBufferIdx] !=
                              currQVRFrameIndex);
                LogSvr("fetchingQVRCameraFrame-2-bGoodFrame: %d  %d  %d  ",gAppContext->qvrNewBufferIdx,currQVRFrameIndex,bGoodFrame);
                if (bGoodFrame) {
                    //res = nXrClient->NXRCameraDevice_GetFrame(&currQVRFrameIndex, QVRCAMERA_MODE_BLOCKING,QVRCAMERA_MODE_NEWER_IF_AVAILABLE,&currQVRFrame);
                    res = nXrClient->NXRCameraDevice_GetFrame(&currQVRFrameIndex, QVRCAMERA_MODE_NON_BLOCKING,QVRCAMERA_MODE_NEWER_IF_AVAILABLE,&currQVRFrame);
                    bGoodFrame = res == QVR_CAM_SUCCESS;
                    LogSvr("fetchingQVRCameraFrame-3-bGoodFrame-res :%d %d %d %d",res,bGoodFrame,currQVRFrameIndex,currQVRFrame.fn);
                }
            }

            if (bGoodFrame) {
                LogSvr("fetchingQVRCameraFrame-4-bGoodFrame: %d",bGoodFrame);
                gAppContext->qvrNewBufferIdx =
                        (gAppContext->qvrNewBufferIdx + 1) % QVR_CAMERA_BUFFER_NUM;
                memcpy(gQVRCameraFrameDataArray[gAppContext->qvrNewBufferIdx].dataArray,
                       currQVRFrame.buffer,
                       currQVRFrame.len);
                LogSvr("gQVRCameraFrameDataArray(stride:%u,format:%u)",currQVRFrame.stride,currQVRFrame.format);
                gQVRCameraFrameDataArray[gAppContext->qvrNewBufferIdx].frameIndex = currQVRFrame.fn;
                gQVRFrameIndexArray[gAppContext->qvrNewBufferIdx] = currQVRFrameIndex;
                {
                    std::unique_lock<std::mutex> autoLock(gAppContext->qvrCameraMutex);
                    gAppContext->qvrAvailableBufferIdx = gAppContext->qvrNewBufferIdx;
                }
                LogSvr("fetchingQVRCameraFrame-4-1-bGoodFrame: %d",bGoodFrame);
                nXrClient->NXRCameraDevice_ReleaseFrame(currQVRFrame.fn);

                LogSvr("fetchingQVRCameraFrame-5-bGoodFrame: %d",gAppContext->qvrNewBufferIdx);
            } else {
                //Vera::CpuTimer::getInstance()->sleep(1e6);
            }
            lastFetchNano = startNano;
        }
    }
    LogSvr("fetchingQVRCameraFrame out of loop");
}

extern "C"
{
SxrResult svrInitQVRCameraA() {
    LOGI("svrInitQVRCamera start gUseQVRCamera=%d", gUseQVRCamera);
    if (!gUseQVRCamera) {
        LOGI("svrInitQVRCamera return directly for gUseQVRCamera=false");
        return SXR_ERROR_NONE;
    }
    if (nullptr == gAppContext->qvrCameraClient) {
        gAppContext->qvrCameraClientHandle =  nXrClient->NXRCameraClient_Create();                  //xiaoyang
        LOGI("svrInitQVRCamera.gAppContext->qvrCameraClientHandle:%d",gAppContext->qvrCameraClientHandle);
    }
    if (gUseQVRCamera) {
        LOGI("svrApiCore.cpp svrBeginVr raise qvrCameraStartSignal");
        gAppContext->qvrCameraStartSignal.raiseSignal();
    }
    gAppContext->qvrCameraApiVersion = nXrClient->NXRCameraClient_GetVersion();                     //xiaoyang
    LOGI("svrInitQVRCamera qvrCameraApiVersion=%d", gAppContext->qvrCameraApiVersion);
    LOGI("svrInitQVRCamera.NXRCameraClient_AttachCamera...................1");
    nXrClient->NXRCameraDevice_DetachCamera();
    gAppContext->qvrDeviceClientHandle = nXrClient->NXRCameraClient_AttachCamera(QVRSERVICE_CAMERA_NAME_TRACKING);
    LOGI("svrInitQVRCamera.gAppContext->qvrDeviceClientHandle:%d",gAppContext->qvrDeviceClientHandle);
    LOGI("svrInitQVRCamera.NXRCameraClient_AttachCamera...................2");
    QVRCAMERA_CAMERA_STATUS cameraStatus = QVRCAMERA_CAMERA_ERROR;
//    int32_t res = QVRCameraDevice_GetCameraState(gAppContext->qvrDeviceClient, &cameraStatus);
    int32_t res = nXrClient->NXRCameraDevice_GetCameraState(&cameraStatus);
    if (QVR_CAM_SUCCESS != res) {
        LOGI("svrInitQVRCamera Failed for QVRCameraDevice_GetCameraState res=%d", res);
        return SXR_ERROR_UNKNOWN;
    }
    LOGI("svrInitQVRCamera cameraStatus=%d", cameraStatus);
    char paramVal[2] = {0};
    res = nXrClient->NXRCameraDevice_GetParamNum(QVR_CAMDEVICE_UINT16_CAMERA_FRAME_WIDTH,
                                                 QVRCAMERA_PARAM_NUM_TYPE_UINT16,
                                                 sizeof(uint16_t),
                                                 paramVal);
    if (QVR_CAM_SUCCESS != res) {
        LOGI("svrInitQVRCamera Failed for QVRCameraDevice_GetParamNum FRAME_WIDTH res=%d", res);
        return SXR_ERROR_UNKNOWN;
    }
    gAppContext->qvrCameraWidth = *reinterpret_cast<uint16_t *>(paramVal);

    LOGI("svrInitQVRCamera qvrCameraWidth(1)=%u", gAppContext->qvrCameraWidth);
    res = nXrClient->NXRCameraDevice_GetParamNum(QVR_CAMDEVICE_UINT16_CAMERA_FRAME_HEIGHT,
                                                 QVRCAMERA_PARAM_NUM_TYPE_UINT16,
                                                 sizeof(uint16_t),
                                                 paramVal);
    if (QVR_CAM_SUCCESS != res) {
        LOGI("svrInitQVRCamera Failed for QVRCameraDevice_GetParamNum FRAME_WIDTH res=%d", res);
        return SXR_ERROR_UNKNOWN;
    }
    gAppContext->qvrCameraWidth =1280;
    gAppContext->qvrCameraHeight = *reinterpret_cast<uint16_t *>(paramVal);
    LOGI("svrInitQVRCamera qvrCameraWidth=%u, qvrCameraHeight=%u", gAppContext->qvrCameraWidth,gAppContext->qvrCameraHeight);
    for (int bufferIdx = 0; bufferIdx != QVR_CAMERA_BUFFER_NUM; ++bufferIdx) {
        gQVRCameraFrameDataArray[bufferIdx].dataArray = new uint8_t[gAppContext->qvrCameraWidth *
                                                                    gAppContext->qvrCameraHeight];
        gQVRFrameIndexArray[bufferIdx] = -1;
    }
    gAppContext->qvrCameraThread = std::thread(fetchingQVRCameraFrame);
    LOGI("svrInitQVRCamera done-------------------");
    return SXR_ERROR_NONE;
}

void SvrInitializeEventA() {
    NxrMoudle interface;
    bool op = interface.open("libnxrsensor_client.so");
    if (interface.isOpen()) {
        LogSvr("initXrClient.interface.open %d\n", op);
    }
    nXrClient = (android::INxrSensorClient *) interface.getNxrClient();
    bool ret = nXrClient->NXR_Init();
    LogSvr("initXrClient.nXrClient->NXR_Init %d\n", ret);
    QVRSERVICE_TRACKING_MODE  currentMode;
    uint32_t  supports;
    int qRes = nXrClient->NXRServiceClient_GetTrackingMode(&currentMode,&supports);
    LogSvr("NXRServiceClient_GetTrackingMode.nXrClient->NXR_Init %d %d\n", currentMode,supports);
//    if(qRes != QVR_SUCCESS || currentMode!=QVRSERVICE_TRACKING_MODE::TRACKING_MODE_POSITIONAL){
//        qRes = nXrClient->NXRServiceClient_SetTrackingMode((QVRSERVICE_TRACKING_MODE) 3);
//    }
    nXrClient->NXRServiceClient_SetTrackingMode((QVRSERVICE_TRACKING_MODE) 3);   //---------------------------xiaoyang
    gAppContext = new SvrAppContext();
    //-----------------------------------------双眼
    outFrameData = new uint8_t[imageWidth*imageHeight];
    svrInitQVRCameraA();
}

SxrResult svrGetLatestCameraBinocularDataA(bool &outBUpdate, uint32_t &outFrameIndex,
                                          uint64_t &outFrameExposureNano,
                                          uint8_t *outLeftFrameData,
                                          uint8_t *outRightFrameData) {
    outBUpdate = false;
    int availableIndex = -1;
    //LOGI("svrGetLatestCameraBinocularData autoLock");
    {
        std::unique_lock<std::mutex> autoLock(gAppContext->qvrCameraMutex);
        availableIndex = gAppContext->qvrAvailableBufferIdx;
    }
    if (-1 == availableIndex || -1 == gQVRFrameIndexArray[availableIndex]) {
        LOGI("svrGetLatestCameraBinocularData skip for availableIndex=%d", availableIndex);
        return SXR_ERROR_NONE;
    }
    if (outFrameIndex != gQVRFrameIndexArray[availableIndex]) {
        // copy left half qvr camera data
        int halfWidth = gAppContext->qvrCameraWidth / 2;
        for (int idx = 0; idx != gAppContext->qvrCameraHeight; ++idx) {
            //LOGI("svrGetLatestCameraBinocularData-outLeftFrameData-%d %s",idx * halfWidth,outLeftFrameData + idx * halfWidth);
            memcpy(outLeftFrameData + idx * halfWidth,
                   gQVRCameraFrameDataArray[availableIndex].dataArray +
                   idx * gAppContext->qvrCameraWidth, sizeof(uint8_t) * halfWidth);
            memcpy(outRightFrameData + idx * halfWidth,
                   gQVRCameraFrameDataArray[availableIndex].dataArray +
                   halfWidth + idx * gAppContext->qvrCameraWidth, sizeof(uint8_t) * halfWidth);

        }
        outFrameIndex = gQVRFrameIndexArray[availableIndex];
        LOGI("fetchingQVRCameraFrame-6-outFrameIndex: %d",outFrameIndex);
        outFrameExposureNano = gQVRCameraFrameDataArray[availableIndex].exposureNano;
        outBUpdate = true;
    }
    return SXR_ERROR_NONE;
}

//-----------------------------------------------------------------------------xiaoyang
SxrResult svrGetLatestQVRCameraDataA(bool &outBUpdate, uint32_t &outCurrFrameIndex,
                                    uint64_t &outFrameExposureNano, uint8_t *outFrameData,
                                    float *outTRDataArray) {
    outBUpdate = false;
    int availableIndex = -1;
    {
        std::unique_lock<std::mutex> autoLock(gAppContext->qvrCameraMutex);
        availableIndex = gAppContext->qvrAvailableBufferIdx;
    }
    if (-1 == availableIndex || -1 == gQVRFrameIndexArray[availableIndex]) {
        LOGI("sxrGetLatestCameraFrameData skip for availableIndex=%d", availableIndex);
        return SXR_ERROR_NONE;
    }
    if (outCurrFrameIndex != gQVRFrameIndexArray[availableIndex]) {
        memcpy(outFrameData, gQVRCameraFrameDataArray[availableIndex].dataArray,
               gAppContext->qvrCameraWidth * gAppContext->qvrCameraHeight);
        outCurrFrameIndex = gQVRFrameIndexArray[availableIndex];

        outBUpdate = true;
//        LOGI("TEST svrGetLatestQVRCameraData outBUpdate=true outCurrFrameIndex=%u outFrameExposureNano=%llu fn=%u", outCurrFrameIndex, outFrameExposureNano, gQVRCameraFrameDataArray[availableIndex].frameIndex);
    }
    return SXR_ERROR_NONE;
}

SxrResult svrDestroyQVRCameraA() {
    LOGI("svrDestroyQVRCamera start gUseQVRCamera=%d", gUseQVRCamera);
    if (!gUseQVRCamera) {
        return SXR_ERROR_NONE;
    }
    nXrClient->NXRCameraDevice_Stop();//xiaoyang.4.0.4
    if (gUseQVRCamera) {
        LOGI("svrApiCore.cpp svrEndVr clear qvrCameraStartSignal");
        gAppContext->qvrCameraStartSignal.clearSignal();
    }
    gAppContext->qvrCameraStartSignal.raiseSignal();
    gAppContext->qvrCameraTerminalSignal.raiseSignal();
    gAppContext->qvrCameraThread.join();
    //QVRCameraDevice_DetachCamera(gAppContext->qvrDeviceClient);
    nXrClient->NXRCameraDevice_DetachCamera();
    gAppContext->qvrDeviceClient = nullptr;
    //QVRCameraClient_Destroy(gAppContext->qvrCameraClient);
    nXrClient->NXRCameraClient_Destroy();
    gAppContext->qvrCameraClient = nullptr;
    // willie change end 2020-8-12
    LOGI("svrDestroyQVRCamera done");
    return SXR_ERROR_NONE;
}

void SvrShutdownEventA() {
    LogDebug("SvrShutdownEvent!");
    nXrClient->NXR_Deinit();
    nXrInterface.close();
    //------------------------------------------------------------------------
    LogSvr("sxrShutdown");
    svrDestroyQVRCameraA();//---------------------xiaoyang
    delete [] outFrameData;
}

}
#endif //_SVR_UNITY_H_

extern "C"
JNIEXPORT void JNICALL
Java_com_nolovr_vrlib_XYCamera_initXyCameara(JNIEnv *env, jclass clazz) {
    // TODO: implement initXyCameara()
    SvrInitializeEventA();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nolovr_vrlib_XYCamera_shutDown(JNIEnv *env, jclass clazz) {
    // TODO: implement shutDown()
    SvrShutdownEventA();
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_nolovr_vrlib_XYCamera_getCamearaData(JNIEnv *env, jclass clazz) {
    // TODO: implement getCamearaData()
    outBUdate= false;
    SxrResult result = svrGetLatestQVRCameraDataA(outBUdate, outCurrFrameIndex,
                                                 outFrameExposureNano, outFrameData,
                                                 outTRDataArray);

    LogSvr("xiaoyang-svrGetLatestCameraBinocularData: %d %d %d %ld", result,outBUdate, outCurrFrameIndex,
           outFrameExposureNano);

    if(outBUdate){
        jbyte *by = (jbyte*)outFrameData;

        jbyteArray jarray = env->NewByteArray(imageWidth*imageHeight);

        env->SetByteArrayRegion(jarray, 0, imageWidth*imageHeight, by);

        return  jarray;
    }else{
        return NULL;
    }
}





















