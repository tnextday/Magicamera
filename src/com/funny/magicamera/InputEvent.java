package com.funny.magicamera;

import com.funny.magicamera.utils.Pool;

import java.util.ArrayList;

/**
 * User: tNextDay
 * Description:
 */
public class InputEvent {

    ArrayList<TouchEvent> touchEvents = new ArrayList<TouchEvent>();
    Pool<TouchEvent> usedTouchEvents = new Pool<TouchEvent>(16, 1000) {
		protected TouchEvent newObject () {
			return new TouchEvent();
		}
	};

    private InputProcessor processor;


    public void setInputProcessor (InputProcessor processor) {
        synchronized (this) {
            this.processor = processor;
        }
    }

    void processEvents () {
        synchronized (this) {
            if (processor != null) {
                final InputProcessor processor = this.processor;

                int len = touchEvents.size();
                for (int i = 0; i < len; i++) {
                    TouchEvent e = touchEvents.get(i);
                    switch (e.type) {
                    case TouchEvent.TOUCH_DOWN:
                        processor.touchDown(e.x, e.y, e.pointer);
                        break;
                    case TouchEvent.TOUCH_UP:
                        processor.touchUp(e.x, e.y, e.pointer);
                        break;
                    case TouchEvent.TOUCH_DRAGGED:
                        processor.touchDragged(e.x, e.y, e.pointer);
                    }
                    usedTouchEvents.free(e);
                }
            } else {
                int len = touchEvents.size();
                for (int i = 0; i < len; i++) {
                    TouchEvent e = touchEvents.get(i);
                    usedTouchEvents.free(e);
                }
            }
            touchEvents.clear();
        }
    }

    public void postTouchEvent(int type, int x, int y, int pointer){
        synchronized (this) {
			TouchEvent event = usedTouchEvents.obtain();
			event.pointer = 0;
			event.x = x;
			event.y = y;
			event.type = type;
			touchEvents.add(event);
		}
    }


    class TouchEvent {
		static final int TOUCH_DOWN = 0;
		static final int TOUCH_UP = 1;
		static final int TOUCH_DRAGGED = 2;

//		long timeStamp;
		int type;
		int x;
		int y;
		int pointer;
	}

    public interface InputProcessor {
        /**
         * Called when the screen was touched or a mouse button was pressed.
         *
         * @param x The x coordinate, origin is in the upper left corner
         * @param y The y coordinate, origin is in the upper left corner
         * @param pointer the pointer for the event.
         * @return whether the input was processed
         */
        public boolean touchDown (int x, int y, int pointer);

        /**
         * Called when a finger was lifted or a mouse button was released.
         *
         * @param x The x coordinate
         * @param y The y coordinate
         * @param pointer the pointer for the event.
         * @return whether the input was processed
         */
        public boolean touchUp (int x, int y, int pointer);

        /**
         * Called when a finger or the mouse was dragged.
         *
         * @param x The x coordinate
         * @param y The y coordinate
         * @param pointer the pointer for the event.
         * @return whether the input was processed
         */
        public boolean touchDragged (int x, int y, int pointer);
    }
}
