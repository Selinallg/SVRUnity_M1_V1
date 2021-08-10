package com.nolovr.vrlib;

import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.util.Log;

import java.io.IOException;

public class PassThroughCamera {
    public static final String TAG = "VrCamera";
    SurfaceTexture cameraTexture;

    Camera camera;

    boolean gotFirstFrame;

    boolean previewStarted =false;

    long	appPtr = 0;	// this must be cached for the onFrameAvailable callback :(

    boolean	hackVerticalFov = false;	// 60 fps preview forces 16:9 aspect, but doesn't report it correctly
    long	startPreviewTime;


    public void setSurfaceTexture(SurfaceTexture paramSurfaceTexture){
        cameraTexture = paramSurfaceTexture;
    }

    public void enableCameraPreview(){

        if(!previewStarted){
            startCameraPreview();
        }
    }

    public void disableCameraPreview()
    {
        if (previewStarted)
            stopCameraPreview();
    }

    public void startCameraPreview(){
        startPreviewTime = System.nanoTime();

        if(cameraTexture==null){
            return ;
        }

        if(camera!=null){
            gotFirstFrame = false;
            camera.startPreview();
            previewStarted = true;
            return;
        }

        Log.d(TAG,"startCameraPreview.Camera.open 01");
        camera = Camera.open();
//        Log.d(TAG,"startCameraPreview.camera.getParameters");
//        Camera.Parameters parms = camera.getParameters();
//
//        Log.d(TAG,"startCameraPreview.camera.getParameters 1");
//        //check if the device supports vr mode preview
//        if ("true".equalsIgnoreCase(parms.get("vrmode-supported"))){
//
//            Log.v(TAG, "VR Mode supported!");
//            //set vr mode
//            parms.set("vrmode", 1);
//            // true if the apps intend to record videos using MediaRecorder
//            parms.setRecordingHint(true);
//            this.hackVerticalFov = true;	// will always be 16:9
//            parms.setPreviewSize(960, 540);
//            parms.set("fast-fps-mode", 2);
//            parms.setPreviewFpsRange(120000, 120000);
//            parms.set("focus-mode", "continuous-video");
//        }    else
//        {
//
//            Camera.Size preferredSize = parms.getPreferredPreviewSizeForVideo();
//            Log.v(TAG, "preferredSize: " + preferredSize.width + " x " + preferredSize.height );
//            List localList1 = parms.getSupportedPreviewFormats();
//            for (int i = 0; i < localList1.size(); i++);
//            List localList2 = parms.getSupportedPreviewSizes();
//            for (int j = 0; j < localList2.size(); j++);
//            float f1 = parms.getHorizontalViewAngle();
//            float f2 = parms.getVerticalViewAngle();
//            Log.v(TAG, "camera view angles:" + f1 + " " + f2);
//            parms.setPreviewSize(640, 480);
//            List localList3 = parms.getSupportedPreviewFpsRange();
//            for (int k = 0; k < localList3.size(); k++);
//            int[] arrayOfInt = (int[])localList3.get(localList3.size() - 1);
//            this.hackVerticalFov= false;
//            parms.setPreviewFpsRange(arrayOfInt[0], arrayOfInt[1]);
//        }
//        Log.d(TAG,"startCameraPreview.camera.setParameters ");
//        camera.setParameters(parms);
        try {

            Log.d(TAG,"startCameraPreview.camera.setPreviewTexture");
            Camera.Parameters parms = camera.getParameters();
            float fovH = parms.getHorizontalViewAngle();
            float fovV = parms.getVerticalViewAngle();
            Log.d(TAG,"startCameraPreview.camera.setPreviewTexture "+fovH+"  "+fovV);
            camera.setPreviewTexture(cameraTexture);
        } catch (IOException e) {
            Log.v(TAG, "startCameraPreviewToTextureId: setPreviewTexture exception");
        }
        Log.v(TAG, "camera.startPreview");
        gotFirstFrame = false;
        camera.startPreview();
        previewStarted = true;
    }

    public void stopCameraPreview(){

        previewStarted = false;
        if(camera!=null){
            camera.stopPreview();
            camera.release();
            camera=null;
        }
    }
}
