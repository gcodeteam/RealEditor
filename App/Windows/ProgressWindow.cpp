#include "ProgressWindow.h"

wxDEFINE_EVENT(UPDATE_MAX_PROGRESS, wxCommandEvent);
wxDEFINE_EVENT(UPDATE_PROGRESS, wxCommandEvent);
wxDEFINE_EVENT(UPDATE_PROGRESS_DESC, wxCommandEvent);
wxDEFINE_EVENT(UPDATE_PROGRESS_FINISH, wxCommandEvent);

ProgressWindow::ProgressWindow(wxWindow* parent, const wxString& title, const wxString& cancel)
  : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxSize(500, 105), wxCAPTION | wxTAB_TRAVERSAL)
{
  SetIcon(wxICON(#114));
  this->SetSizeHints(wxDefaultSize, wxDefaultSize);
  this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENU));

  wxBoxSizer* bSizer1 = new wxBoxSizer(wxVERTICAL);

  ActionLabel = new wxStaticText(this, wxID_ANY, wxT("Prepearing..."), wxDefaultPosition, wxDefaultSize, 0);
  ActionLabel->Wrap(-1);
  bSizer1->Add(ActionLabel, 1, wxALL | wxEXPAND, 5);

  wxBoxSizer* bSizer2 = new wxBoxSizer(wxHORIZONTAL);

  ProgressBar = new wxGauge(this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL);
  ProgressBar->SetValue(0);
  bSizer2->Add(ProgressBar, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

  CancelButton = new wxButton(this, wxID_ANY, cancel, wxDefaultPosition, wxDefaultSize, 0);
  bSizer2->Add(CancelButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);


  bSizer1->Add(bSizer2, 0, wxEXPAND, 5);


  this->SetSizer(bSizer1);
  this->Layout();

  this->Centre(wxBOTH);

  // Connect Events
  CancelButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ProgressWindow::OnCancellClicked), NULL, this);
}

void ProgressWindow::SetActionText(const wxString& text)
{
  if (!Cancelled.load())
  {
    ActionLabel->SetLabelText(text);
  }
}

void ProgressWindow::SetCurrentProgress(int progress)
{
  if (Cancelled.load())
  {
    return;
  }
  if (progress < 0)
  {
    ProgressBar->Pulse();
  }
  else
  {
    ProgressBar->SetValue(progress);
  }
}


void ProgressWindow::SetMaxProgress(int max)
{
  ProgressBar->SetRange(max);
}

void ProgressWindow::SetCanCancel(bool flag)
{
  CancelButton->Enable(flag);
}

void ProgressWindow::OnCancellClicked(wxCommandEvent&)
{
  CancelButton->Enable(false);
  Cancelled.store(true);
  ActionLabel->SetLabelText("Stopping...");
  ProgressBar->Pulse();
}

void ProgressWindow::OnUpdateMaxProgress(wxCommandEvent& e)
{
  SetMaxProgress(e.GetInt());
}

void ProgressWindow::OnUpdateProgress(wxCommandEvent& e)
{
  SetCurrentProgress(e.GetInt());
}

void ProgressWindow::OnUpdateProgressDescription(wxCommandEvent& e)
{
  SetActionText(e.GetString());
}

void ProgressWindow::OnUpdateProgressFinish(wxCommandEvent& e)
{
  SetCurrentProgress(0);
  CancelButton->Enable(false);
  if (IsModal())
  {
    EndModal(e.GetInt());
  }
  else
  {
    Close();
  }
}

wxBEGIN_EVENT_TABLE(ProgressWindow, wxDialog)
EVT_COMMAND(wxID_ANY, UPDATE_PROGRESS, OnUpdateProgress)
EVT_COMMAND(wxID_ANY, UPDATE_MAX_PROGRESS, OnUpdateMaxProgress)
EVT_COMMAND(wxID_ANY, UPDATE_PROGRESS_DESC, OnUpdateProgressDescription)
EVT_COMMAND(wxID_ANY, UPDATE_PROGRESS_FINISH, OnUpdateProgressFinish)
wxEND_EVENT_TABLE()