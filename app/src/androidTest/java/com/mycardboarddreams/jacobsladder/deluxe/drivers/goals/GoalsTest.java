package com.mycardboarddreams.jacobsladder.deluxe.drivers.goals;

import org.junit.Test;

import static org.assertj.core.api.Java6Assertions.assertThat;

public class GoalsTest {

	@Test
	public void testCreateGoals(){
		assertThat(runGoalsTests()).isEqualTo("No error");
	}
	
	public static native String runGoalsTests();
}
