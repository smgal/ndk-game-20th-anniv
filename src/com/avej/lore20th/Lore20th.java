
package com.avej.lore20th;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.media.MediaPlayer;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Toast;

class GameConfig
{
	final static int BUFFER_WIDTH = 1280;
	final static int BUFFER_HEIGHT = 720;
	
	static boolean is_terminating = false;
	
	static int    touch_x = 0;
	static int    touch_y = 0;
	
	static int    prev_canvas_w = BUFFER_WIDTH;
	static int    prev_canvas_h = BUFFER_HEIGHT;
	
	static Bitmap src_bitmap;
	static Rect   src_rect = new Rect(0, 0, GameConfig.BUFFER_WIDTH, GameConfig.BUFFER_HEIGHT);
	static Rect   dst_rect = new Rect();
	static double scaling_factor_x = 1.0;
	static double scaling_factor_y = 1.0;
	static long   start_time = 0;
	
	static MediaPlayer media_player;
}

public class Lore20th extends Activity
{
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

		requestWindowFeature(Window.FEATURE_NO_TITLE);
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		
		GameConfig.is_terminating = false;
		
		YozoraView yozoraView = new YozoraView(this);
		setContentView(yozoraView);
		
		GameTask task = new GameTask(this, yozoraView);
		task.execute();
	}

	@Override
	protected void onDestroy()
	{
		// TODO Auto-generated method stub
		super.onDestroy();
	}
	
	@Override
	protected void onResume()
	{
		super.onResume();
		
		// temporary play for test
		GameConfig.media_player = MediaPlayer.create(Lore20th.this, R.raw.lore_1);
		GameConfig.media_player.setLooping(true);
		GameConfig.media_player.start();
	}

	@Override
	protected void onPause()
	{
		super.onPause();
		
		GameConfig.media_player.stop();
		GameConfig.media_player.release();
	}

	@Override
	public void onBackPressed()
	{
		// TODO Auto-generated method stub
		// super.onBackPressed();
		GameConfig.is_terminating = true;
	}

	@Override
	public boolean onTouchEvent(MotionEvent event)
	{
		int motion_event = event.getAction(); 

		if (motion_event == MotionEvent.ACTION_DOWN)
		{
			int ax = (int)event.getX();
			int ay = (int)event.getY();

			GameConfig.touch_x = ax;
			GameConfig.touch_y = ay;
		}
		else if (motion_event == MotionEvent.ACTION_MOVE)
		{
			int ax = (int)event.getX();
			int ay = (int)event.getY();

			GameConfig.touch_x = ax;
			GameConfig.touch_y = ay;
		}
		else if (motion_event == MotionEvent.ACTION_UP)
		{
			GameConfig.touch_x = -1;
			GameConfig.touch_y = -1;
		}

		return true;
	}

    /* load native library */
	static
	{
		System.loadLibrary("lore20th");
	}
}

class YozoraView extends View
{
	/* implemented by liblore20th.so */
	public static native void initYozora(String resource_path, String app_name);
	public static native void doneYozora();
	public static native int  renderYozora(Bitmap bitmap, long time_ms, int motion_x, int motion_y);

	public YozoraView(Context context)
	{
		super(context);
		
		GameConfig.src_bitmap = Bitmap.createBitmap(GameConfig.BUFFER_WIDTH, GameConfig.BUFFER_HEIGHT, Bitmap.Config.ARGB_8888);

		GameConfig.start_time = System.currentTimeMillis();
	}

	@Override protected void onDraw(Canvas canvas)
	{
		GameConfig.prev_canvas_w = canvas.getWidth();
		GameConfig.prev_canvas_h = canvas.getHeight();
		
		//?? lock bitmap
		canvas.drawBitmap(GameConfig.src_bitmap, GameConfig.src_rect, GameConfig.dst_rect, null);
		//?? unlock bitmap
	}
}

class GameTask extends AsyncTask<Void, Void, Void>
{
	private Activity activity;
	private View view;
	
	private int saved_canvas_w = 0;
	private int saved_canvas_h = 0;
	
	GameTask(Activity activity, View view)
	{
		this.activity = activity;
		this.view = view;
	}
	
	@Override
	protected Void doInBackground(Void... params)
	{
		PackageInfo info = null;

		try
		{
			info = view.getContext().getPackageManager().getPackageInfo("com.avej.lore20th", 0);
		}
		catch (NameNotFoundException e)
		{
			Log.e("[SMGAL]", e.toString());
			return null;
		}
		
		YozoraView.initYozora(info.applicationInfo.sourceDir, "lore20th");
		
		while (!GameConfig.is_terminating)
		{
			if (saved_canvas_w != GameConfig.prev_canvas_w || saved_canvas_h != GameConfig.prev_canvas_h)
			{
				saved_canvas_w = GameConfig.prev_canvas_w;
				saved_canvas_h = GameConfig.prev_canvas_h;

				if (GameConfig.src_rect.width() * saved_canvas_h > GameConfig.src_rect.height() * saved_canvas_w)
				{
					int dst_width = saved_canvas_w;
					int dst_height = GameConfig.src_rect.height() * saved_canvas_w / GameConfig.src_rect.width();
					int dst_x = (saved_canvas_w - dst_width) / 2;
					int dst_y = (saved_canvas_h - dst_height) / 2;
					
					GameConfig.dst_rect.set(dst_x, dst_y, dst_x + dst_width, dst_y + dst_height);
				}
				else
				{
					int dst_width = GameConfig.src_rect.width() * saved_canvas_h / GameConfig.src_rect.height();
					int dst_height = saved_canvas_h;
					int dst_x = (saved_canvas_w - dst_width) / 2;
					int dst_y = (saved_canvas_h - dst_height) / 2;

					GameConfig.dst_rect.set(dst_x, dst_y, dst_x + dst_width, dst_y + dst_height);
				}

				GameConfig.scaling_factor_x = 1.0 * GameConfig.src_rect.width() / GameConfig.dst_rect.width();
				GameConfig.scaling_factor_y = 1.0 * GameConfig.src_rect.height() / GameConfig.dst_rect.height();
			}

			{
				int revised_touch_x = GameConfig.touch_x;
				int revised_touch_y = GameConfig.touch_y;

				if (revised_touch_x >= 0 && revised_touch_y >= 0 )
				{
					revised_touch_x -= GameConfig.dst_rect.left;
					revised_touch_y -= GameConfig.dst_rect.top;
					
					revised_touch_x = (int)((double)revised_touch_x * GameConfig.scaling_factor_x);
					revised_touch_y = (int)((double)revised_touch_y * GameConfig.scaling_factor_y);
				}
				
				//?? lock bitmap
				int result = YozoraView.renderYozora(GameConfig.src_bitmap, System.currentTimeMillis() - GameConfig.start_time, revised_touch_x, revised_touch_y);
				//?? unlock bitmap
		
				if (result == 0)
					break;
			}

			this.publishProgress();
			
			try
			{
				Thread.sleep(20);
			}
			catch (InterruptedException e)
			{
				e.printStackTrace();
			}

			android.util.Log.i("[SMGAL]", "loop");
		}
		
		YozoraView.doneYozora();
		
		return null;
	}

	@Override
	protected void onProgressUpdate(Void... values)
	{
		view.invalidate();
	}

	@Override
	protected void onPostExecute(Void result)
	{
		this.activity.finish();
		//??System.exit(0);
	}
	
}
