#include <Core/CoreAll.h>
#include <Fusion/FusionAll.h>
#include <CAM/CAMAll.h>

#include <fstream>

using namespace adsk::core;
using namespace std;

Ptr<UserInterface> ui;

string getControls(Ptr<Application> app, Ptr<ToolbarControls> controls, int level, bool fullReport, string result);

extern "C" XI_EXPORT bool run(const char* context)
{
	Ptr<Application> app = Application::get();
	if (!app)
		return false;

	ui = app->userInterface();
	if (!ui)
		return false;

	DialogResults dialogResult = ui->messageBox("Create a full report (list and control details) or \nminimal report (control list only)?", "UI Report Type", MessageBoxButtonTypes::YesNoCancelButtonType, MessageBoxIconTypes::QuestionIconType);
	bool fullReport = false;
	if (DialogResults::DialogYes == dialogResult)
	{
		fullReport = true;
	}
	else if (DialogResults::DialogNo == dialogResult)
	{
		fullReport = false;
	}
	else
	{
		return false;
	}

	Ptr<FileDialog> fileDialog = ui->createFileDialog();
	if (!fileDialog)
		return false;

	fileDialog->isMultiSelectEnabled(false);
	fileDialog->title("Specify result filename");
	fileDialog->filter("Text files (*.txt)");
	fileDialog->filterIndex(0);
	dialogResult = fileDialog->showSave();
	string filename;
	if (DialogResults::DialogOK == dialogResult)
	{
		filename = fileDialog->filename();
	}
	else
	{
		return false;
	}

	Ptr<Toolbars> toolBars = ui->toolbars();
	if (!toolBars)
		return false;

	Ptr<Toolbar> qatToolbar = toolBars->itemById("QAT");
	if (!qatToolbar)
		return false;

	// Display the name of all of the existing toolbars.
	string result = "** Toolbars (" + to_string(toolBars->count()) + ") **\r\n";
	for (Ptr<Toolbar> toolBar : toolBars)
	{
		if (!toolBar)
			continue;

		Ptr<ToolbarControls> toolBarCtrls = toolBar->controls();
		if (!toolBarCtrls)
			continue;

		string msg = "   " + toolBar->id() + " (" + to_string(toolBarCtrls->count()) + ")____\r\n";
		result += msg;
		result = getControls(app, toolBarCtrls, 2, fullReport, result);
	}

	Ptr<ToolbarPanelList> toolBarPanels = ui->allToolbarPanels();
	if (!toolBarPanels)
		return false;

	result += "\r\n\r\n** Toolbar Panels (" + to_string(toolBarPanels->count()) + ")____\r\n";
	for (Ptr<ToolbarPanel> toolBarPanel : toolBarPanels)
	{
		if (!toolBarPanel)
			continue;

		Ptr<ToolbarControls> toolBarCtrls = toolBarPanel->controls();
		if (!toolBarCtrls)
			continue;

		string msg = "   " + toolBarPanel->id() + " (" + to_string(toolBarCtrls->count()) + ")____\r\n";
		msg += string("      Visible: ") + (toolBarPanel->isVisible() ? "true" : "false") + "\r\n";
		msg += "      Index: " + to_string(toolBarPanel->index()) + "\r\n";
		result += msg;
		result = getControls(app, toolBarCtrls, 2, fullReport, result);
	}

	Ptr<Workspaces> workspaces = ui->workspaces();
	if (!workspaces)
		return false;

	result += "\r\n\r\n** Workspaces  (" + to_string(workspaces->count()) + ")____\r\n";
	for (Ptr<Workspace> workspace : workspaces)
	{
		if (!workspace)
			continue;

		if (workspace->productType() == "")
			continue;

		Ptr<ToolbarPanels> panelsForWorkspace = workspace->toolbarPanels();
		if (!panelsForWorkspace)
			continue;

		string msg = "   " + workspace->id() + " (" + to_string(panelsForWorkspace->count()) + ")____\r\n";
		result += msg;
		for (Ptr<ToolbarPanel> toolBarPanel : panelsForWorkspace)
		{
			string msg = "      " + toolBarPanel->id() + "\r\n";
			result += msg;
		}
	}

	ofstream filestrm(filename);
	filestrm << result;
	filestrm.close();
	ui->messageBox("File written to \"" + filename + "\"");

	return true;
}

// Function used to recursively traverse the controls.
string getControls(Ptr<Application> app, Ptr<ToolbarControls> controls, int level, bool fullReport, string result)
{
	string indentStr = "";
	for (int i = 0; i < level * 3; ++i)
		indentStr.append(" ");

	for (Ptr<ToolbarControl> control : controls)
	{
		if (!control)
			continue;

		if (Ptr<SeparatorControl> separatorCtrl = control)
		{
			string msg = indentStr + "-Separator-\n";
			msg += indentStr + "   " + "Control ID: " + control->id() + "\n";
			msg += indentStr + "   " + "isVisible: " + (control->isVisible() ? "true" : "false") + "\n";
			msg += indentStr + "   " + "index: " + to_string(control->index()) + "\n";
			result += msg;
		}
		else
		{
			if (Ptr<CommandControl> cmdCtrl = control)
			{
				string msg = indentStr + "Command Control____\n";
				msg += indentStr + "   " + "Control ID: " + control->id() + "\n";
				msg += indentStr + "   " + "isVisible: " + (control->isVisible() ? "true" : "false") + "\n";
				msg += indentStr + "   " + "index: " + to_string(control->index()) + "\n";

				if (fullReport)
				{
					bool bPromoted = cmdCtrl->isPromoted();
					if (app->getLastError() == 0)
						msg += indentStr + "   " + "isPromoted: " + (bPromoted ? "true" : "false") + "\n";
					bool bPromotedByDefault = cmdCtrl->isPromotedByDefault();
					if (app->getLastError() == 0)
						msg += indentStr + "   " + "isPromotedByDefault: " + (cmdCtrl->isPromotedByDefault() ? "true" : "false") + "\n";
					msg += indentStr + "   " + "Command Definition____\n";
					Ptr<CommandDefinition> commandDef = cmdCtrl->commandDefinition();
					if (!commandDef)
					{
						msg += indentStr + "      " + "Unable to get the associated CommandDefinition.\n";
						result += msg;
						continue;
					}

					msg += indentStr + "      " + "Definition ID: " + commandDef->id() + "\n";

					msg += indentStr + "      " + "Control Definition____\n";

					Ptr<ControlDefinition> controlDef = commandDef->controlDefinition();
					if (!controlDef)
					{
						msg += indentStr + "      " + "Unable to get the associated ControlDefinition.\n";
						result += msg;
						continue;
					}

					msg += indentStr + "         " + "Name: " + controlDef->name() + "\n";
					msg += indentStr + "         " + "isEnabled: " + (controlDef->isEnabled() ? "true" : "false") + "\n";
					msg += indentStr + "         " + "isVisible: " + (controlDef->isVisible() ? "true" : "false") + "\n";

					if (Ptr<ButtonControlDefinition> buttonCtrlDef = controlDef)
						msg += indentStr + "         " + "Type: ButtonControlDefinition\n";
					else if (Ptr<CheckBoxControlDefinition> checkBoxCtrlDef = controlDef)
					{
						msg += indentStr + "         " + "Type: CheckBoxControlDefinition\n";
						msg += indentStr + "         " + "isChecked: " + (checkBoxCtrlDef->isChecked() ? "true" : "false") + "\n";
					}
					else if (Ptr<ListControlDefinition> listCtrlDef = controlDef)
					{
						msg += indentStr + "         " + "Type: ListControlDefinition\n";
						string displayType;
						if (listCtrlDef->listControlDisplayType() == ListControlDisplayTypes::CheckBoxListType)
							displayType = "Check box list";
						else if (listCtrlDef->listControlDisplayType() == ListControlDisplayTypes::RadioButtonlistType)
							displayType = "Radio button list";
						else if (listCtrlDef->listControlDisplayType() == ListControlDisplayTypes::StandardListType)
							displayType = "Standard list";

						msg += indentStr + "         " + "Displayed as: " + displayType + "\n";

						Ptr<ListItem> lastSelectedItem = listCtrlDef->lastSelected();
						if (!lastSelectedItem)
						{
							msg += indentStr + "      " + "Unable to get the active selected item.\n";
							result += msg;
							continue;
						}

						msg += indentStr + "         " + "lastSelected: " + lastSelectedItem->name() + "\n";
						msg += indentStr + "         " + "List Items\n";

						Ptr<ListItems> listItems = listCtrlDef->listItems();
						if (!listItems)
						{
							msg += indentStr + "      " + "Unable to get the associated ListItems.\n";
							result += msg;
							continue;
						}

						for (Ptr<ListItem> listItem : listItems)
						{
							if (!listItem)
								continue;

							if (listItem->isSeparator())
								msg += indentStr + "            " + "-Separator-\n";
							else
								msg += indentStr + "            " + "name: " + listItem->name() + ", isSelected: " + (listItem->isSelected() ? "true" : "false") + "\n";
						}
					}
					else
					{
						msg += indentStr + "         " + "**Unexpected type of control definition.\n";
					}
				}
				result += msg;
			}
			else if (Ptr<DropDownControl> dropDownCtrl = control)
			{
				Ptr<ToolbarControls> subCtrls = dropDownCtrl->controls();
				if (!subCtrls)
					continue;

				string msg = indentStr + "Drop Down Control (" + to_string(subCtrls->count()) + ")____\n";
				msg += indentStr + "   " + "ID: " + control->id() + "\n";
				result += msg;
				result = getControls(app, subCtrls, level + 1, fullReport, result);
			}
			else if (Ptr<SplitButtonControl> splitButtonCtrl = control)
			{
				string msg = indentStr + "Split Button Control____\n";
				msg += indentStr + "   " + "ID: " + control->id() + "\n";
				msg += indentStr + "   " + "isVisible: " + (control->isVisible() ? "true" : "false") + "\n";

				if (fullReport)
				{
					msg += indentStr + "   " + "isLastUsedShown: " + (splitButtonCtrl->isLastUsedShown() ? "true" : "false") + "\n";
					Ptr<CommandDefinition> defaultCmdDef = splitButtonCtrl->defaultCommandDefinition();
					if (!defaultCmdDef)
					{
						result += msg;
						continue;
					}

					msg += indentStr + "   " + "default Command: " + defaultCmdDef->id() + "\n";
					msg += indentStr + "   " + "+ Additional associated controls:\n";

					vector<Ptr<CommandDefinition> > additionalDefs = splitButtonCtrl->additionalDefinitions();
					for (Ptr<CommandDefinition> additionalDef : additionalDefs)
					{
						if (!additionalDef)
							continue;
						msg += indentStr + "      " + additionalDef->id() + "\n";
					}
				}
				result += msg;
			}
			else
			{
				string ctype = control->objectType();
				size_t nPos = ctype.rfind("::");
				if (string::npos == nPos)
					continue;
				ctype = ctype.substr(nPos + 1, ctype.length() - 1 - nPos);
				string msg = indentStr + "*****" + control->id() + ", type: " + ctype + ", visible: " + (control->isVisible() ? "true" : "false") + "\n";
				result += msg;
			}
		}
	}

	return result;
}

#ifdef XI_WIN

#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hmodule, DWORD reason, LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

#endif // XI_WIN