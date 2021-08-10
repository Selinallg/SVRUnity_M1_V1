//
// Created by Mark on 2021/4/8.
//

#ifndef INXRSENSOR_CLIENT_H
#define INXRSENSOR_CLIENT_H

#include <stdlib.h>
#include "NxrSensorTypes.h"
#define NOLO_SONIC_HEIGHT 70001
#define NOLO_SONIC_RECENTER_TYPE 70002
using namespace nxr_client;

namespace android {
    class INxrSensorClient {
    public:
        virtual ~INxrSensorClient(){};

        virtual bool init() = 0;
        virtual int32_t getClientVersion() = 0;
        virtual int32_t getServiceVersion() = 0;
        virtual void pause() = 0;
        virtual void resume() = 0;
        virtual void setTrackingMode(int32_t mode) = 0;
        virtual bool getHeadInfo(long prediction,float *dest,int length) = 0;
        virtual bool getHeadEuler(long prediction,double *dest,int length) = 0;
        virtual void destroy() = 0;
        virtual void reset() = 0;
        virtual bool is6Dof() = 0;
        virtual bool isSupportMatrix() = 0;
        virtual void setParameterI(int key,int value) = 0;
        virtual int getParameterI(int key) = 0;
        virtual void updateFeedData(void *feed) = 0;
        virtual bool getFeedData(void *dest) = 0;
        virtual bool getTrackingData(void *dest) = 0;
        virtual bool getNxrRawData(void *dest) = 0;
        virtual bool isSupportSlam() = 0;
        virtual void setParameterIA(int key,int *values,int length) = 0;
        virtual int getParameterIA(int key,int *result,int length) = 0;
        virtual bool switchVRMode(int mode) = 0;
        virtual void updateNxrPredictedData(NxrPredictedData *data) = 0;
        virtual bool getNxrPredictedData(NxrPredictedData *data) = 0;
        virtual void recenter(float *position) = 0;//[0]-x,[1]-y,[2]-z (右手坐标系)
        //for nxrslam
        virtual bool initNxrSlam() = 0;
        virtual void pauseNxrSlam() = 0;
        virtual void resumeNxrSlam() = 0;
        virtual void destroyNxrSlam() = 0;
        virtual void setNxrSlamParameterI(int key,int value) = 0;
        virtual int getNxrSlamParameterI(int key) = 0;
        virtual int getNxrSlamPoseData(float *result,int length) = 0;
        //for vsc
        virtual bool initVSC() = 0;
        virtual void uninitVSC() = 0;
        virtual bool startStreaming(StreamType type,void(*f)(StreamType , NxrStreamData *)) = 0;
        virtual bool stopStreaming(StreamType type) = 0;
        //for qvr api
        virtual bool NXR_Init() = 0;
        virtual void NXR_Deinit() = 0;
        virtual int NXRCameraClient_GetVersion() = 0;
        virtual int NXRCameraClient_Create() = 0;
        virtual int NXRCameraClient_CreateWithKey(const char *szKey) = 0;
        virtual void NXRCameraClient_Destroy() = 0;
        virtual int NXRCameraClient_SetClientStatusCallback(camera_client_status_callback_fn cb,void *ctx) = 0;
        virtual int NXRCameraClient_AttachCamera(const char *cameraName) = 0;
        virtual int NXRCameraClient_AttachCameraWithParams(const char* pCameraName,qvr_plugin_param_t pParams[], int32_t nParams) = 0;//xiaoyang.4.0.4
        virtual int NXRCameraClient_InitRingBufferData(QVRSERVICE_RING_BUFFER_ID id) = 0;
        virtual int NXRCameraClient_WriteRingBufferData(QVRSERVICE_RING_BUFFER_ID id, void *data, uint32_t length) = 0;
        virtual int NXRCameraClient_GetParam(const char *name, uint32_t *length, char *value) = 0;
        virtual int NXRCameraClient_SetParam(const char *name, const char *value) = 0;
        virtual int NXRCameraClient_RegisterForClientNotification(QVRCAMERA_CLIENT_NOTIFICATION notification,camera_client_notification_callback_fn cb,void *ctx) = 0;
        virtual void NXRCameraDevice_DetachCamera() = 0;
        virtual int NXRCameraDevice_GetCameraState(QVRCAMERA_CAMERA_STATUS *pState) = 0;
        virtual int NXRCameraDevice_SetCameraStatusCallback(camera_status_callback_fn cb,void *ctx) = 0;
        virtual int NXRCameraDevice_GetParam(const char *name, uint32_t *length, char *value) = 0;
        virtual int NXRCameraDevice_SetParam(const char *name, const char *value) = 0;
        virtual int NXRCameraDevice_GetParamNum(const char *name, QVRCAMERA_PARAM_NUM_TYPE type, int length, char *value) = 0;
        virtual int NXRCameraDevice_SetParamNum(const char *name, QVRCAMERA_PARAM_NUM_TYPE type, int length, char *value) = 0;
        virtual int NXRCameraDevice_Start() = 0;
        virtual int NXRCameraDevice_Stop() = 0;
        virtual int NXRCameraDevice_SetExposureAndGain(uint64_t exposure_ns, int iso_gain) = 0;
        virtual int NXRCameraDevice_GetCurrentFrameNumber(int32_t *fn) = 0;
        virtual int NXRCameraDevice_GetFrame(int32_t *fn, QVRCAMERA_BLOCK_MODE block, QVRCAMERA_DROP_MODE drop, qvrcamera_frame_t *pframe) = 0;
        virtual int NXRCameraDevice_ReleaseFrame(int32_t fn) = 0;
        virtual int NXRCameraDevice_SetCropRegion(uint32_t l_top, uint32_t l_left, uint32_t l_width,
                                                  uint32_t l_height,
                                                  uint32_t r_top, uint32_t r_left, uint32_t r_width,
                                                  uint32_t r_height) = 0;
        virtual int NXRCameraDevice_SetGammaCorrectionValue(float gamma) = 0;
        virtual int NXRCameraDevice_RegisterForCameraNotification(QVRCAMERA_DEVICE_NOTIFICATION notification,camera_device_notification_callback_fn cb,void *ctx) = 0;
        virtual const char *NXRCameraHelper_ShowState(QVRCAMERA_CAMERA_STATUS state) = 0;
        virtual const char *NXRCameraHelper_ShowCamError(int error) = 0;
        //QVRServiceClient
        virtual int NXRServiceClient_SetClientStatusCallback(client_status_callback_fn cb,void *ctx) = 0;
        virtual QVRSERVICE_VRMODE_STATE NXRServiceClient_GetVRMode() = 0;
        virtual int NXRServiceClient_GetTrackingMode(QVRSERVICE_TRACKING_MODE *pCurrentMode, uint32_t *pSupportedModes) = 0;
        virtual int NXRServiceClient_SetTrackingMode(QVRSERVICE_TRACKING_MODE mode) = 0;
        virtual int NXRServiceClient_SetThreadPriority(int tid, int policy, int priority) = 0;
        virtual int NXRServiceClient_GetParam(const char *name, uint32_t *length, char *value) = 0;
        virtual int NXRServiceClient_SetParam(const char *name, const char *value) = 0;
        virtual int NXRServiceClient_GetHeadTrackingData(qvrservice_head_tracking_data_t **ppData) = 0;
        virtual int NXRServiceClient_SetDisplayInterruptCapture(QVRSERVICE_DISP_INTERRUPT_ID id, uint32_t mode) = 0;
        virtual int NXRServiceClient_SetThreadAttributesByType(int tid, QVRSERVICE_THREAD_TYPE thread_type) = 0;
        virtual int NXRServiceClient_SetOperatingLevel(qvrservice_perf_level_t *perf_levels,
                                                       uint32_t num_perf_levels, char *reserved1,
                                                       uint32_t *reserved2) = 0;
        virtual int NXRServiceClient_GetEyeTrackingMode(uint32_t *pCurrentMode, uint32_t *pSupportedModes) = 0;
        virtual int NXRServiceClient_SetEyeTrackingMode(uint32_t mode) = 0;
        virtual int NXRServiceClient_SetDisplayInterruptConfig(QVRSERVICE_DISP_INTERRUPT_ID id, void *pCfg, uint32_t cfgSize) = 0;
        virtual int NXRServiceClient_GetSensorRawData(qvrservice_sensor_data_raw_t **ppData) = 0;
        virtual int NXRServiceClient_GetRingBufferDescriptor(QVRSERVICE_RING_BUFFER_ID id, qvrservice_ring_buffer_desc_t *pDesc) = 0;
        virtual int NXRServiceClient_GetHistoricalHeadTrackingData(qvrservice_head_tracking_data_t **ppData,
                                                                   int64_t timestampNs) = 0;
        virtual int NXRServiceClient_GetDisplayInterruptTimestamp(QVRSERVICE_DISP_INTERRUPT_ID id, qvrservice_ts_t **ppTs) = 0;
        virtual int NXRServiceClient_RegisterForNotification(QVRSERVICE_CLIENT_NOTIFICATION notification,notification_callback_fn cb,void *ctx) = 0;
        virtual int NXRServiceClient_GetEyeTrackingData(qvrservice_eye_tracking_data_t **ppData, int64_t timestampNs) = 0;
        virtual int NXRServiceClient_ActivePredictedHeadTrackingPoseElement(int16_t *element_id,
                                                                            int64_t target_prediction_timestamp_ns) = 0;
        virtual int NXRServiceClient_SetTransformationMatrix(QVRSERVICE_TRANSFORMATION_MATRIX_TYPE type, float mat4x4[16]) = 0;
        virtual int NXRServiceClient_GetHwTransforms(uint32_t *pnTransforms, qvrservice_hw_transform_t transforms[]) = 0;
        virtual int NXRServiceClient_GetHwTransform(qvrservice_hw_transform_t *transform) = 0;
        virtual int NXRServiceClient_GetPluginDataHandle(const char *pluginName) = 0;
        virtual void NXRServiceClient_ReleasePluginDataHandle() = 0;
        //QVRPluginData
        virtual int NXRPluginData_GetPluginDataInfo(qvr_plugin_info_t *pluginInfo) = 0;
        virtual int NXRPluginData_GetData(const char *pControl, uint32_t controlLen,
                                          char *pPayload, uint32_t *pPayloadLen) = 0;
        virtual int NXRPluginData_SetData(const char *pControl, uint32_t controlLen,
                                          const char *pPayload, uint32_t payloadLen) = 0;
        //QVRServicePlugin
        virtual int NXRServicePlugin_GetData(char *pControl, uint32_t controlLen,
                                             char *pPayload, uint32_t *pPayloadLen) = 0;
        virtual int NXRServicePlugin_SetData(const char *pControl, uint32_t controlLen,
                                             const char *pPayload, uint32_t payloadLen) = 0;
        virtual int NXRServicePlugin_Create(qvr_plugin_param_t *params, int32_t nParams,
                                            qvrservice_hw_transform_t transforms[],
                                            int32_t nTransforms) = 0;
        virtual void NXRServicePlugin_Destroy() = 0;
        virtual void NXRServicePluginCallbacks_NotifyError(QVRSERVICE_PLUGIN_ERROR_STATE error_state,uint64_t param) = 0;
        virtual int NXRServicePlugin_Init(const char *key) = 0;
        virtual int NXRServicePlugin_Deinit() = 0;
        virtual int NXRServicePlugin_Start() = 0;
        virtual int NXRServicePlugin_Stop() = 0;
        virtual int NXRServicePlugin_SetTransform(qvrservice_hw_transform_t *transform) = 0;
        virtual int NXRServicePlugin_GetParam(const char *pName, uint32_t *pLen, char *pValue) = 0;
        virtual int NXRServicePlugin_SetParam(const char *pName, const char *pValue) = 0;
    };
};//nxr_client

#endif //INXRSENSOR_CLIENT_H
