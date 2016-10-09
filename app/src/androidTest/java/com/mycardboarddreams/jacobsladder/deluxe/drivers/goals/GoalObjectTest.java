package com.mycardboarddreams.jacobsladder.deluxe.drivers.goals;

import android.util.Log;

import com.mycardboarddreams.jacobsladder.deluxe.drivers.CubeDriverTest;

import org.junit.Test;

import static org.assertj.core.api.Java6Assertions.assertThat;

public class GoalObjectTest {

	private static final String LOG_TAG = CubeDriverTest.class.getSimpleName();
	
	static {
		Log.d(LOG_TAG, "Loading native library");
		System.loadLibrary("native-lib");
    	Log.d(LOG_TAG, "Done loading native library");
	}

	@Test
	public void testCreateGoalObjects(){
		assertThat(runGoalObjectTests(17, 17, 35)).isEqualTo("No error");
	}
	
	//----------------------------
	public static native String runGoalObjectTests(int x_blocks, int y_blocks, int z_blocks);
}
