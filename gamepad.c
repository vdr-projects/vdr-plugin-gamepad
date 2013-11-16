/*
 * gamepad.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */
#include <vdr/plugin.h>
#include "gamepad_process_keys_thread.h"
#include "gamepad_setup.h"

static const char *VERSION        = "0.0.1";

class cPluginGamepad : public cPlugin {
private:
  cGamepadProcessKeysThread *processKeysThread;

public:
  cPluginGamepad(void);
  virtual ~cPluginGamepad();
  virtual const char *Version(void) { return VERSION; }
  virtual const char *Description(void) { return tr("Use a joystick device to control VDR"); }
  virtual const char *CommandLineHelp(void);
  virtual bool ProcessArgs(int argc, char *argv[]);
  virtual bool Initialize(void);
  virtual bool Start(void);
  virtual void Stop(void);
  virtual void Housekeeping(void);
  virtual void MainThreadHook(void);
  virtual cString Active(void);
  virtual time_t WakeupTime(void);
  //virtual const char *MainMenuEntry(void) { return MAINMENUENTRY; }
  virtual cOsdObject *MainMenuAction(void);
  virtual cMenuSetupPage *SetupMenu(void);
  virtual bool SetupParse(const char *Name, const char *Value);
  virtual bool Service(const char *Id, void *Data = NULL);
  virtual const char **SVDRPHelpPages(void);
  virtual cString SVDRPCommand(const char *Command, const char *Option, int &ReplyCode);
  };

cPluginGamepad::cPluginGamepad(void) : processKeysThread(NULL)
{
  // Initialize any member variables here.
  // DON'T DO ANYTHING ELSE THAT MAY HAVE SIDE EFFECTS, REQUIRE GLOBAL
  // VDR OBJECTS TO EXIST OR PRODUCE ANY OUTPUT!
}

cPluginGamepad::~cPluginGamepad()
{
  // Clean up after yourself!
}

const char *cPluginGamepad::CommandLineHelp(void)
{
  // Return a string that describes all known command line options.
  return NULL;
}

bool cPluginGamepad::ProcessArgs(int argc, char *argv[])
{
  // Implement command line argument processing here if applicable.
  return true;
}

bool cPluginGamepad::Initialize(void)
{
  processKeysThread = new cGamepadProcessKeysThread();
  processKeysThread->loadKeyMap();
  return true;
}

bool cPluginGamepad::Start(void)
{
  if (processKeysThread == NULL)
  {
    bool ret;
    ret = processKeysThread->Start();
    dsyslog("[gamepad] start handler returned %d", ret);
  }
  return true;
}

void cPluginGamepad::Stop(void)
{
  if (processKeysThread) {
    processKeysThread->Cancel(10);
    processKeysThread = NULL;
  }
}

void cPluginGamepad::Housekeeping(void)
{
  // Perform any cleanup or other regular tasks.
}

void cPluginGamepad::MainThreadHook(void)
{
  // Perform actions in the context of the main program thread.
  // WARNING: Use with great care - see PLUGINS.html!
}

cString cPluginGamepad::Active(void)
{
  // Return a message string if shutdown should be postponed
  return NULL;
}

time_t cPluginGamepad::WakeupTime(void)
{
  // Return custom wakeup time for shutdown script
  return 0;
}

cOsdObject *cPluginGamepad::MainMenuAction(void)
{
  // Perform the action when selected from the main VDR menu.
  return NULL;
}

cMenuSetupPage *cPluginGamepad::SetupMenu(void)
{
  return new cGamepadSetup();
}

bool cPluginGamepad::SetupParse(const char *Name, const char *Value)
{
  if      (!strcasecmp(Name, "JoystickDevice")) strn0cpy(gamepadConfig.device, Value, sizeof(gamepadConfig.device));
  else if (!strcasecmp(Name, "TestMode"))       gamepadConfig.testMode = atoi(Value);
  else    return false;

  return true;
}

bool cPluginGamepad::Service(const char *Id, void *Data)
{
  // Handle custom service requests from other plugins
  return false;
}

const char **cPluginGamepad::SVDRPHelpPages(void)
{
  static const char *HelpPages[] = {
    "ENABLE\n"
    "    Enables the plugin.",
    "DISABLE\n"
    "    Disables the plugin.",
    NULL
    };
  return HelpPages;
}

cString cPluginGamepad::SVDRPCommand(const char *Command, const char *Option, int &ReplyCode)
{
  if (strcasecmp(Command, "ENABLE") == 0)
  {
     ReplyCode = 900;
     return "Enabled";
  }
  else if (strcasecmp(Command, "DISABLE") == 0)
  {
     Stop();
     ReplyCode = 900;
     return "Disabled";
  }
  return NULL;
}

VDRPLUGINCREATOR(cPluginGamepad); // Don't touch this!
