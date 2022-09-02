#include "GetFileFromNet.h"
#include "avmsgebox.h"
#include <wx\progdlg.h>
#include <wx\url.h>
#include <wx\stream.h>
#include <wx\wfstream.h>
#include <wx\sstream.h>

extern wxString workingDir;

#define BUFSIZE 0x10000

void DownloadFileFromNet(wxWindow* window)
{
	/*
#ifdef __WIN64__
	wxString urlstr = wxT("http://www.av.eg1.in/download/EG Anti Virus2017/Update/EGAV2017_WIN7x64_UPDATE.zip");
#else
	wxString urlstr = wxT("http://www.av.eg1.in/download/EG Anti Virus2017/Update/EGAV2017_WIN7x86_UPDATE.zip");
#endif
	wxURL url(urlstr);

	wxString filename = urlstr.AfterLast('/');

	if (url.GetError() == wxURL_NOERR)
	{
		wxInputStream *in = url.GetInputStream();

		if (in && in->IsOk())
		{
			unsigned char tempbuf[BUFSIZE];
			wxMemoryBuffer buf;

			size_t total_len = in->GetSize();
			size_t data_loaded = 0;
			bool abort = false;

			wxProgressDialog progress(wxT("Downloading..."), wxT("Download in progress"), total_len, window, wxPD_CAN_ABORT);
			wxFileOutputStream* localFile = new wxFileOutputStream(workingDir + wxT("\\") + filename);
			while (in->CanRead() && !in->Eof() && !abort)
			{
				in->Read(tempbuf, BUFSIZE);
				size_t readlen = in->LastRead();
				//write buffer to file
				localFile->WriteAll(tempbuf, BUFSIZE);

				if (readlen>0)
				{
					buf.AppendData(tempbuf, readlen);
					data_loaded += readlen;
				}

				if (total_len>0)
				{
					// if we know the length of the file, display correct progress
					wxString msg;
					msg.Printf(wxT("Downloaded %ld of %ld bytes"), data_loaded, total_len);
					abort = !progress.Update(data_loaded, msg);
				}
				else
				{
					// if we don't know the length of the file, just Pulse
					abort = !progress.Pulse();
				}
			}

			if (abort)
			{
				wxLogMessage(wxT("Download was cancelled."));
			}
			else
			{
				// wxMemoryBuffer buf now contains the downloaded data
				wxLogMessage(wxT("Downloaded %ld bytes"), buf.GetDataLen());
			}
		}
		delete in;
	}
	*/
	
}

