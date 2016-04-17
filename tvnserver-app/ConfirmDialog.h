#ifndef _CONFIRM_DIALOG_H_
#define _CONFIRM_DIALOG_H_

#include "gui/BaseDialog.h"
#include "gui/Control.h"

#include "TvnServerApplication.h"

class TvnServerApplication;

class ConfirmDialog : public BaseDialog
{
public:
  ConfirmDialog(TvnServerApplication *tvnServerApplication);
  ~ConfirmDialog();

protected:
  BOOL onInitDialog();
  BOOL onCommand(UINT controlID, UINT notificationID);

private:
  TvnServerApplication *m_tvnServerApplication;

  Control m_text;
};

#endif

