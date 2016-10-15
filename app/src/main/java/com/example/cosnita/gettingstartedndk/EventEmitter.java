package com.example.cosnita.gettingstartedndk;

public class EventEmitter {
    private long emitterPtrId;
    private EventEmitter(long emitterPtrId) {
        this.emitterPtrId = emitterPtrId;
    }

    public long getEmitterPtrId() {
        return emitterPtrId;
    }

    public native static EventEmitter getInstance();

    public native void on(String evtName, EventEmitterAction action);

    public native void emit(String evtName, String evtData);
}
