#include "ConfirmDialog.h"

#include "tvnserver/resource.h"

ConfirmDialog::ConfirmDialog(TvnServerApplication *tvnServerApplication)
: BaseDialog(IDD_CONFIRMDIALOG),
  m_tvnServerApplication(tvnServerApplication)
{
}

ConfirmDialog::~ConfirmDialog()
{
}

BOOL ConfirmDialog::onInitDialog()
{
  setControlById(m_text, IDC_CONFIRM_TEXT);

  StringStorage text;
  text.format(_T("管理员%s需要控制您的电脑，确定？"), m_tvnServerApplication->getUser().getString());
  m_text.setText(text.getString());
  return TRUE;
}

BOOL ConfirmDialog::onCommand(UINT controlID, UINT notificationID)
{
  switch (controlID) {
  case IDOK:
    m_tvnServerApplication->onConfirm(true);
    kill(0);
    break;

  case IDCANCEL:
    m_tvnServerApplication->onConfirm(false);
    kill(0);
    break;

  default:
    _ASSERT(true);
    return FALSE;
  }
  return TRUE;
}
