#include "Dynamo.h"

#include <sstream>

#include "DynamoManagedWrapper\DynamoManagedWrapper.h"



const std::string commandIdOnModelWorkspace = "launchCommandOnWorkspaceDynamo";
const std::string commandIdOnPanel = "launchCommandOnPanelDynamo";
const std::string iconResources = "./resources";

Ptr<Application> app;
Ptr<UserInterface> ui;

class CommandExecutedHandler : public adsk::core::CommandEventHandler
{
public:
	void notify(const Ptr<CommandEventArgs>& eventArgs) override
	{
		Ptr<Event> firingEvent = eventArgs->firingEvent();
		if (!firingEvent)
			return;

		Ptr<Command> command = firingEvent->sender();
		if (!command)
			return;

		Ptr<CommandDefinition> parentDefinition = command->parentCommandDefinition();
		if (!parentDefinition)
			return;

		/*std::stringstream ss;
		ss << "command: " + parentDefinition->id() << " executed successfully";

		ui->messageBox(ss.str());*/
		const char* asm_location = 
			"C:\\Users\pratapa.ADS\\AppData\\Local\\Autodesk\\webdeploy\\production\\c076e205c02e6f8f9c6eac7820ccca6953c0c53a";
		DynamoManagedWrapper::LoadDynamo(asm_location);
		
	}
};

class CommandCreatedOnModelWorkspaceHandler : public adsk::core::CommandCreatedEventHandler
{
public:
	void notify(const Ptr<CommandCreatedEventArgs>& eventArgs) override
	{
		if (eventArgs)
		{
			Ptr<Command> command = eventArgs->command();
			if (!command)
				return;

			Ptr<CommandEvent> exec = command->execute();
			if (!exec)
				return;
			exec->add(&onCommandExecuted_);

			if (ui)
				ui->messageBox("QAT command created successfully");
		}
	}
private:
	CommandExecutedHandler onCommandExecuted_;
} onCommandCreatedOnModelWorkspace;

extern "C" XI_EXPORT bool run(const char* context)
{
	const std::string commandName = "Dynamo";
	const std::string commandDescription = "Launch Dynamo";
	const std::string commandResources = "./resources";

	app = Application::get();
	if (!app)
		return false;

	ui = app->userInterface();
	if (!ui)
		return false;

	Ptr<CommandDefinitions> commandDefinitions = ui->commandDefinitions();
	if (!commandDefinitions)
		return false;

	ui->messageBox("running!");

	// Object Model Reference:
	// http://help.autodesk.com/cloudhelp/ENU/Fusion-360-API/images/Fusion.pdf
	//
	// add a command button on Quick Access Toolbar
	//
	Ptr<Toolbars> toolbars = ui->toolbars();
	if (!toolbars)
		return false;

	Ptr<Toolbar> toolbarModelWorkspace = toolbars->itemById("QAT");
	if (!toolbarModelWorkspace)
		return false;

	Ptr<ToolbarControls> toolbarControlsModelWorkspace = toolbarModelWorkspace->controls();
	if (!toolbarControlsModelWorkspace)
		return false;
	Ptr<ToolbarControl> toolbarControlModelWorkspace = toolbarControlsModelWorkspace->itemById(commandIdOnPanel);
	if (!toolbarControlModelWorkspace)
	{	
		Ptr<CommandDefinition> commandDefinitionModelWorkspace = commandDefinitions->itemById(commandIdOnPanel);
		if (!commandDefinitionModelWorkspace)
		{
			commandDefinitionModelWorkspace = commandDefinitions->addButtonDefinition(commandIdOnPanel, commandName, commandDescription, commandResources);

		}

		Ptr<CommandCreatedEvent> cmdCreatedEvent = commandDefinitionModelWorkspace->commandCreated();
		if (!cmdCreatedEvent)
			return false;
		cmdCreatedEvent->add(&onCommandCreatedOnModelWorkspace);
		toolbarControlModelWorkspace = toolbarControlsModelWorkspace->addCommand(commandDefinitionModelWorkspace);
		if (!toolbarControlsModelWorkspace)
			return false;
		
		toolbarControlModelWorkspace->isVisible(true);
		ui->messageBox("A Dynamo command button is successfully added to the Quick Access Toolbar");

		//
		// add a new panel in modeling workspace
		//
		Ptr<Workspaces> workspaces = ui->workspaces();
		if (!workspaces)
			return false;
		Ptr<Workspace> modelingWorkspace = workspaces->itemById("FusionSolidEnvironment");
		if (!modelingWorkspace)
			return false;
		Ptr<ToolbarPanels> toolbarPanels = modelingWorkspace->toolbarPanels();
		if (!toolbarPanels)
			return false;
		Ptr<ToolbarControls> toolbarControlsPanel = toolbarPanels->add("DYN", "Dynamo", "10", true)->controls();
		if (!toolbarControlsPanel)
			return false;
		Ptr<ToolbarControl> toolbarControlPanel = toolbarControlsPanel->itemById(commandIdOnModelWorkspace);
		if (!toolbarControlPanel)
		{
			Ptr<CommandDefinition> commandDefinitionPanel = commandDefinitions->itemById(commandIdOnModelWorkspace);
			if (!commandDefinitionPanel)
			{
				commandDefinitionPanel = commandDefinitions->addButtonDefinition(commandIdOnModelWorkspace, commandName, commandDescription, commandResources);
			}
			Ptr<CommandCreatedEvent> cmdCreatedEvent = commandDefinitionPanel->commandCreated();
			if (!cmdCreatedEvent)
				return false;
			cmdCreatedEvent->add(&onCommandCreatedOnModelWorkspace);
			toolbarControlPanel = toolbarControlsPanel->addCommand(commandDefinitionPanel);
			if (toolbarControlPanel)
				toolbarControlPanel->isVisible(true);
			ui->messageBox("A Dynamo command is successfully added to the panel in modeling workspace");
		}
	}

	return true;
}

extern "C" XI_EXPORT bool stop(const char* context)
{
	if (ui)
	{
		ui->messageBox("in stop");
		ui = nullptr;
	}

	return true;
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
