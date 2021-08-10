package com.chirpmicro.usbglue;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbManager;
import android.util.Log;
import java.util.Collection;
import java.util.HashMap;
import java.util.Iterator;

public class UsbHelper
{
    private static final String TAG = UsbHelper.class.getSimpleName();
    private final int usbVendorID;
    private final int usbProductID;
    private final Context context;
    private UsbDeviceConnection usbDeviceConnection = null;
    private UsbDevice usbDevice = null;

    private final BroadcastReceiver broadcastReceiver = new BroadcastReceiver()
    {
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();

            if ("android.hardware.usb.action.USB_DEVICE_ATTACHED".equals(action)) {
                UsbDevice device = (UsbDevice)intent.getParcelableExtra("device");
                if ((device != null) &&
                        (device.getVendorId() == UsbHelper.this.usbVendorID) && (device.getProductId() == UsbHelper.this.usbProductID)) {
                    Log.d(UsbHelper.TAG, "USB device attached.");
                    UsbHelper.this.onUsbDeviceAttached();
                }
            }
            else if ("android.hardware.usb.action.USB_DEVICE_DETACHED".equals(action)) {
                UsbDevice device = (UsbDevice)intent.getParcelableExtra("device");
                if ((device != null) &&
                        (device.getVendorId() == UsbHelper.this.usbVendorID) && (device.getProductId() == UsbHelper.this.usbProductID)) {
                    Log.d(UsbHelper.TAG, "USB device detached.");
                    UsbHelper.this.onUsbDeviceDetached();
                }
            }
        }
    };

    public native void onUsbDeviceAttached();

    public native void onUsbDeviceDetached();

    public UsbHelper(Context context, int usbVendorID, int usbProductID)
    {
        this.context = context;
        this.usbVendorID = usbVendorID;
        this.usbProductID = usbProductID;
        IntentFilter filter = new IntentFilter();
        filter.addAction("android.hardware.usb.action.USB_DEVICE_DETACHED");
        filter.addAction("android.hardware.usb.action.USB_DEVICE_ATTACHED");
        context.registerReceiver(this.broadcastReceiver, filter);
    }

    public void close()
    {
        this.context.unregisterReceiver(this.broadcastReceiver);
    }

    public void closeUsbDevice()
    {
        if (this.usbDeviceConnection != null) {
            this.usbDeviceConnection.close();
            this.usbDeviceConnection = null;
            this.usbDevice = null;
        }
    }

    public int getUsbFileDescriptor() {
        return this.usbDeviceConnection.getFileDescriptor();
    }

    public String getUsbDevicePath() {
        return this.usbDevice.getDeviceName();
    }

    public boolean isUsbDeviceOpen() {
        return this.usbDeviceConnection != null;
    }

    public int openUsbDevice()
    {
        UsbManager usbManager = (UsbManager)this.context.getSystemService(Context.USB_SERVICE);
        HashMap deviceList = usbManager.getDeviceList();
        Iterator deviceIterator = deviceList.values().iterator();
        while (deviceIterator.hasNext()) {
            UsbDevice device = (UsbDevice)deviceIterator.next();
            if ((device.getVendorId() == this.usbVendorID) && (device.getProductId() == this.usbProductID)) {
                this.usbDeviceConnection = usbManager.openDevice(device);
                if (this.usbDeviceConnection != null) {
                    this.usbDevice = device;
                    Log.d(TAG, "file descriptor: " + this.usbDeviceConnection.getFileDescriptor());
                    Log.d(TAG, "device path: " + this.usbDevice.getDeviceName());
                    return 0;
                }
                return 2;
            }
        }

        return 1;
    }
}