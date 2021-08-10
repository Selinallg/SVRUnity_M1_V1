/******************************************************************************
* File: QVRCameraDeviceParam.h
*
* Copyright (c) 2017-2018 Qualcomm Technologies, Inc.
* All Rights Reserved
* Confidential and Proprietary - Qualcomm Technologies, Inc.
*
******************************************************************************/

#ifndef QVR_CAMERADEVICE_PARAM_H
#define QVR_CAMERADEVICE_PARAM_H

// Getters implemented by camera device, requested by any Client

#define    QVRSERVICE_MAX_CAMERA_DEVICES                       4

// Parameters that can be retrieved from the QVRCamera Client
#define    QVR_CAMCLIENT_QTIME_TO_ANDROID_BOOT_NS      "qvrcamclient-qtime-to-android-boot-ns"

// Set to a QVR_CAMDEVICE_RESOLUTION_MODE value.
#define    QVR_CAMCLIENT_ATTACH_STRING_RES_MODE        "qvrcamclient_resolution_mode"   //xiaoyang

///
/// This camera device will return buffers in the preferred format, if possible.
/// Clients can check the .format member of the qvrcamera_frame_t struct
/// to confirm the frame format. Note that attaching may fail if the preferred
/// format is not supported.
///
#define    QVR_CAMCLIENT_ATTACH_STRING_PREF_FORMAT     "qvrcamclient_preferred_format"

// Parameters that can be retrieved from the QVRCamera Device
#define    QVR_CAMDEVICE_STRING_CAMERA_NAME            "qvrcamdevice_name"
#define    QVR_CAMDEVICE_INT8_CAMERA_ID                "qvrcamdevice_id"
#define    QVR_CAMDEVICE_STRING_CAMERA_DRIVER          "qvrcamdevice_driver"
#define    QVR_CAMDEVICE_UINT8_RING_ID                 "qvrcamdevice_ring_id"
#define    QVR_CAMDEVICE_STRING_SENSOR_NAME            "qvrcamdevice_sensor"
#define    QVR_CAMDEVICE_UINT8_AUTO_EXPOSURE           "qvrcamdevice_auto_exposure"
#define    QVR_CAMDEVICE_UINT16_CAMERA_FRAME_HEIGHT    "qvrcamdevice_frame_height"
#define    QVR_CAMDEVICE_UINT16_CAMERA_FRAME_WIDTH     "qvrcamdevice_frame_width"
#define    QVR_CAMDEVICE_UINT16_CAMERA_FRAME_YSTRIDE   "qvrcamdevice_frame_ystride"
#define    QVR_CAMDEVICE_UINT8_FRAME_RDI_MODE          "qvrcamdevice_frame_rdi_mode"
#define    QVR_CAMDEVICE_UINT8_FRAME_RDI_BIT_DEPTH     "qvrcamdevice_frame_rdi_bit_depth"
#define    QVR_CAMDEVICE_UINT8_HVX_MODE                "qvrcamdevice_hvx_mode"
#define    QVR_CAMDEVICE_UINT8_HVX_STREAM              "qvrcamdevice_hvx_stream"
#define    QVR_CAMDEVICE_STRING_HVX_ALGO               "qvrcamdevice_hvx_algo"
#define    QVR_CAMDEVICE_STRING_HVX_SENSOR_NAME        "qvrcamdevice_hvx_sensor"
#define    QVR_CAMDEVICE_UINT8_CONTROL_PROTECTED       "qvrcamdevice_control_protected"
#define    QVR_CAMDEVICE_UINT8_PRIVILEGED_CLIENT       "qvrcamdevice_privileged_client"
#define    QVR_CAMDEVICE_UINT8_MASTER_ONLY             "qvrcamdevice_master_only"
#define    QVR_CAMDEVICE_STRING_FRAME_LOGGING_MODE     "qvrcamdevice_frame_logging_mode"
#define    QVR_CAMDEVICE_STRING_DEFAULT_FLASH_MODE     "qvrcamdevice_default_flash_mode"
#define    QVR_CAMDEVICE_STRING_DEFAULT_RES_MODE       "qvrcamdevice_default_resolution_mode"
#define    QVR_CAMDEVICE_STRING_RES_MODE               "qvrcamdevice_resolution_mode"  //xiaoyang

#define    QVR_CAMDEVICE_STRING_FRAME_FORMAT           "qvrcamdevice_frame_format"
#define    QVR_CAMDEVICE_STRING_RDI_FRAME_FORMAT       "qvrcamdevice_rdi_frame_format"

#define    QVR_CAMDEVICE_VECT_RESOLUTION               "qvrcamdevice_res"
#define    QVR_CAMDEVICE_VECT_QTR_RESOLUTION           "qvrcamdevice_quarter_res"
#define    QVR_CAMDEVICE_VECT_CALIBRATION_SIZE         "qvrcamdevice_calibration_size"
#define    QVR_CAMDEVICE_VECT_PRINCIPLE_POINT          "qvrcamdevice_principal_point"
#define    QVR_CAMDEVICE_VECT_FOCAL_LENGTH             "qvrcamdevice_focal_length"
#define    QVR_CAMDEVICE_VECT_RADIUS_DISTORTION        "qvrcamdevice_radial_distortion"

// Obtained from the sensor configuration
#define    QVR_CAMDEVICE_UINT64_LINE_READOUT_TIME      "qvrcamdevice_sensor_line_readout_time"
#define    QVR_CAMDEVICE_UINT64_ADC_READOUT_TIME       "qvrcamdevice_sensor_adc_readout_time"

// Initial exposure time and ISO used during  manual exposure control
#define QVR_CAMDEVICE_UINT64_INITIAL_EXPOSURE_TIME_NS  "qvrcamdevice_initial_exposure_time_ns"
#define QVR_CAMDEVICE_UINT32_INITIAL_ISO_GAIN          "qvrcamdevice_initial_iso_gain"

// Set to a QVR_CAMDEVICE_FLASH_MODE value
#define    QVR_CAMDEVICE_STRING_FLASH_MODE             "qvrcamdevice_flash_mode"

// Set to a QVR_CAMDEVICE_RESOLUTION_MODE value
#define    QVR_CAMDEVICE_STRING_RES_MODE               "qvrcamdevice_resolution_mode"

// supported string values for the frame logging mode
#define    QVR_CAMDEVICE_LOGGING_MODE_DISABLED         "disabled"
#define    QVR_CAMDEVICE_LOGGING_MODE_CONTINUOUS       "continuous"
#define    QVR_CAMDEVICE_LOGGING_MODE_ONESHOT          "oneshot"

// supported string values for the camera driver types
#define    QVR_CAMDEVICE_CAMERA_DRIVER_NATIVE          "native" // default
#define    QVR_CAMDEVICE_CAMERA_DRIVER_V4L2            "v4l2"           //xiaoyang
#define    QVR_CAMDEVICE_CAMERA_DRIVER_EXTERNAL        "external"       //xiaoyang

// supported string values for QVR_CAMDEVICE_STRING_DEFAULT_FLASH_MODE and
// QVR_CAMDEVICE_STRING_FLASH_MODE.
// note: torch mode is only valid of auto exposure is disabled.
#define    QVR_CAMDEVICE_FLASH_MODE_OFF                "off"
#define    QVR_CAMDEVICE_FLASH_MODE_TORCH              "torch"

// supported string values for QVR_CAMDEVICE_STRING_DEFAULT_RES_MODE and
// QVR_CAMDEVICE_STRING_RES_MODE.and QVR_CAMCLIENT_ATTACH_STRING_RES_MODE. //xiaoyang
#define    QVR_CAMDEVICE_RESOLUTION_MODE_FULL          "full"
#define    QVR_CAMDEVICE_RESOLUTION_MODE_QUARTER       "quarter"


// supported string values for camera frame format
#define    QVR_CAMDEVICE_FRAME_FORMAT_Y8               "y8"
#define    QVR_CAMDEVICE_FRAME_FORMAT_YUV420           "yuv420"
#define    QVR_CAMDEVICE_FRAME_FORMAT_RAW10            "raw10"
#define    QVR_CAMDEVICE_FRAME_FORMAT_RAW16            "raw16"

// supported string values for QVR_CAMCLIENT_ATTACH_STRING_PREF_FORMAT
#define    QVR_CAMDEVICE_FORMAT_DEFAULT                "default"
#define    QVR_CAMDEVICE_FORMAT_RAW10                  "raw10"        // used to request only raw10 format
#define    QVR_CAMDEVICE_FORMAT_RAW16                  "raw16"        // used to request only raw16 format
#define    QVR_CAMDEVICE_FORMAT_RAW                    "raw"          // used to request any raw format

// supported string values for image arrangement
#define    QVR_CAMDEVICE_IMAGE_ARRANGEMENT_UNKNOWN     "unknown"
#define    QVR_CAMDEVICE_IMAGE_ARRANGEMENT_NONE        "none"
#define    QVR_CAMDEVICE_IMAGE_ARRANGEMENT_HORIZONTAL  "horizontal"
#define    QVR_CAMDEVICE_IMAGE_ARRANGEMENT_VERTICAL    "vertical"

#endif //QVR_CAMERADEVICE_PARAM_H
