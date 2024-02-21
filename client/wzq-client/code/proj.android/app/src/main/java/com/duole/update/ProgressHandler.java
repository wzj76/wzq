package com.duole.update;

public interface ProgressHandler {
	public void progressBegin(final String sTitle, boolean bSilent);
	public void progressFinish();
	public void updateProgress(final String sTitle, int nProgress);
}