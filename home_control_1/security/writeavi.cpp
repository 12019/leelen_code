#include "writeavi.h"

void writeAVI_free(writeAVI *stream)
{
	if (stream->visource != NULL)
		ms_filter_destroy(stream->visource);
	if (stream->ausource != NULL)
		ms_filter_destroy(stream->ausource);
	if (stream->H264encoder != NULL)
		ms_filter_destroy(stream->H264encoder);
	if (stream->G711encoder != NULL)
		ms_filter_destroy(stream->G711encoder);
	if (stream->writeG711 != NULL)
		ms_filter_destroy(stream->writeG711);
	if (stream->writeH264 != NULL)
		ms_filter_destroy(stream->writeH264);
	//if (stream->au_tee != NULL)
	//	ms_filter_destroy(stream->au_tee);
	if (stream->vi_tee != NULL)
		ms_filter_destroy(stream->vi_tee);
	if (stream->videoout != NULL)
		ms_filter_destroy(stream->videoout);
	//if (stream->soundout != NULL)
	//	ms_filter_destroy(stream->soundout);
	if (stream->mfc_pp != NULL)
		ms_filter_destroy(stream->mfc_pp);
	if (stream->ticker != NULL)
		ms_ticker_destroy(stream->ticker);
	if (stream->ticker1 != NULL)
		ms_ticker_destroy(stream->ticker1);
	ms_free (stream);
}

writeAVI *writeAVI_start(MSWebCam *device, MSSndCard *sndcard, char *outfile, int v_channel)
{
	writeAVI *stream = (writeAVI *)ms_new0(writeAVI, 1);
	int tmp;
	tmp = 8000;

	/**************************** create video and audio card filter*****************************8*/
	stream->visource = ms_web_cam_create_reader(device);
	ms_filter_call_method(stream->visource,MS_FILTER_SET_VIDEO_CHANNEL, &v_channel);
	stream->ausource = ms_snd_card_create_reader(sndcard);
	ms_filter_call_method(stream->ausource, MS_FILTER_SET_SAMPLE_RATE, &tmp);

	/**************************** create G711 and H264 recoder filter*****************************8*/
	stream->H264encoder = ms_filter_new(MS_MFC_ENC_ID);
	stream->G711encoder = ms_filter_new(MS_ALAW_ENC_ID);

	/****************************create msaviwrite filter ***********************************/
	stream->writeH264 = ms_filter_new(MS_AVI_H264_ID);
	stream->writeG711 = ms_filter_new(MS_AVI_G711_ID);

	//stream->au_tee = ms_filter_new(MS_TEE_ID);
	stream->vi_tee = ms_filter_new(MS_TEE_ID);

	stream->videoout = ms_filter_new(MS_VIDEO_OUT_VIRT_ID);
	//stream->mixer = ms_filter_new(MS_AUDIO_MIXER_ID); //mixer
	//stream->soundout = ms_snd_card_create_writer(sndcard);
	//tmp = 8000;
	//ms_filter_call_method(stream->soundout,MS_FILTER_SET_SAMPLE_RATE,&tmp);

	stream->mfc_pp = ms_filter_new(MS_MFC_PP_ID);

	if (ms_filter_call_method(stream->writeH264, MS_AVI_WRITE_OPEN, (avi_t *)outfile) < 0){
		//ms_filter_destroy(stream->writeH264);
		//ms_filter_destroy(stream->writeG711);
		//ms_free(stream);
		writeAVI_free(stream);
		return NULL;
	}

	int duration = 30;
	ms_filter_call_method(stream->writeH264, MS_AVI_WRITE_SET_DURATION, &duration);
	ms_filter_call_method_noarg(stream->writeH264, MS_AVI_WRITE_START);

	ms_filter_call_method(stream->writeG711, MS_AVI_WRITE_OPEN_AU, outfile);
	ms_filter_call_method_noarg(stream->writeG711, MS_AVI_WRITE_START_AU);

	/***************************connect all filter ****************************************/
	ms_filter_link(stream->visource,0, stream->vi_tee,0);
	ms_filter_link(stream->vi_tee, 0, stream->H264encoder, 0);
	ms_filter_link(stream->H264encoder, 0, stream->mfc_pp, 0);
	ms_filter_link(stream->mfc_pp, 0, stream->writeH264, 0);
	ms_filter_link(stream->vi_tee, 1, stream->videoout, 0);

	//ms_filter_link(stream->ausource,0, stream->au_tee, 0);
	//ms_filter_link(stream->au_tee, 0, stream->G711encoder, 0);
	ms_filter_link(stream->ausource, 0, stream->G711encoder, 0);
	ms_filter_link(stream->G711encoder, 0, stream->writeG711, 0);
	//ms_filter_link(stream->au_tee, 1, stream->mixer, 0);
	//ms_filter_link(stream->mixer, 0, stream->soundout, 0);

	/* create new ticker */
	stream->ticker = ms_ticker_new();
	ms_ticker_attach(stream->ticker, stream->visource);
	stream->ticker1 = ms_ticker_new();
	ms_ticker_attach(stream->ticker1, stream->ausource);

	return stream;
}

void writeAVI_stop(writeAVI * stream)
{
	ms_ticker_detach(stream->ticker, stream->visource);
	ms_filter_unlink(stream->visource, 0, stream->vi_tee,0);
	ms_filter_unlink(stream->vi_tee, 0, stream->H264encoder, 0);
	ms_filter_unlink(stream->H264encoder, 0, stream->mfc_pp, 0);
	ms_filter_unlink(stream->mfc_pp, 0, stream->writeH264, 0);
	ms_filter_unlink(stream->vi_tee, 1, stream->videoout, 0);

	ms_ticker_detach(stream->ticker1, stream->ausource);
	//ms_filter_unlink(stream->ausource,0, stream->au_tee, 0);
	//ms_filter_unlink(stream->au_tee, 0, stream->G711encoder, 0);
	ms_filter_unlink(stream->ausource, 0, stream->G711encoder, 0);
	ms_filter_unlink(stream->G711encoder, 0, stream->writeG711, 0);
	//ms_filter_link(stream->au_tee, 1, stream->mixer, 0);
	//ms_filter_link(stream->mixer, 0, stream->soundout, 0);

	ms_filter_call_method_noarg(stream->writeH264,MS_AVI_WRITE_CLOSE);

	writeAVI_free(stream);
}
