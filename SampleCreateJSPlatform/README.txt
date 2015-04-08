This file contains guidelines on how to add support for custom platforms in Flash Professional CC using the 'Custom Platform Support Development Kit'.
The sample custom platform provided as part of this package can be used as reference by developers for developing their own custom platform.

1. Software Requirements for 'SampleCreateJSPlatform':
   ===========================================
- Microsoft Windows 7 or Apple Mac OS 10.8 and later
- Microsoft Visual Studio 2012 (for Windows) or XCode 4.5.2
- Adobe Flash Professional CC 2014.2 (Feb/2015)
- Eclipse IDE for C/C++ Developers
- Adobe Extension Builder 3.0
- Adobe Extension Manager CC 7.2.1.6
- Mongoose Web Server (This is needed by 'SampleCreateJSPlatform' to view the published content and your custom plugin might not need this)
- Google Chrome (This is needed by 'SampleCreateJSPlatform' to view the published content and your custom plugin might not need this)

2. Installation steps for 'SampleCreateJSPlatform':
=======================================

a. Ensure that you have installed 'Adobe Flash Professional CC 2014.2'. You can install it using 'Adobe Creative Cloud Application'.
b. Ensure that you have installed 'Adobe Extension Manager CC 7.2.1.6'. You can install it using 'Adobe Creative Cloud Application'.
c. Double click on the 'SampleCreateJSPlatform.zxp' and install the plugin using Adobe Extension Manager CC 
d. Close Adobe Extension Manager CC
e. Launch Flash CC 2014
   >> You will be able to see a new document type called 'SamplePlugin' added to the Welcome Screen and to the New Document dialog (File>New...)

3. Package Contents :
=================

The downloaded package contains an sample implementation of a custom platform 'SampleCreateJSPlatform'. 
This sample platform outputs to CreateJS. This sample can referred by developers to add support for custom platforms of their choice.

It has following contents:

- Plugin : Contains C/C++ source along with the Visual Studio 2012 project and the XCode 4.5.2 projects for the plugin.
- EclipseProject - Contains an Eclipse project that can be used to generate the final platform ZXP. 
- Runtime - Contains a sample runtime that makes use of the 'CreateJS' runtime.
- SampleCreateJSPlatform.zxp - This is the final custom platform package that can be installed using 'Adobe Extension Manager CC'.
- ReadMe.txt - This file contains information about this package.


4. Building, Installing and Running:
===========================

This section talks about the steps to build and install of the SampleCreateJSPlatform. 

4.1 Building the C++ project

a. Ensure that you have Microsoft Visual Studio 2012 (for Windows) or XCode 4.5.2 (for MAC).

b. Make sure you have downloaded ''CustomPlatformDevelopmentKit''. You can get it from 'http://help.adobe.com/flash/using/enabling-support-custom-platforms.html'

c. Copy the folders 'SampleCreateJSPlatform' and 'CustomPlatformDevelopmentKit' to a folder on your system. Both this folders must be in the same level.

d. Open the solution file 'SampleCreateJSPlugin.mp.64.sln' present inside:

SampleCreateJSPlatform->Plugin->SampleCreateJSPlugin->project->win  (for Windows)

(or)

Open 'SampleCreateJSPlugin.mp.xcodeproj' present inside:

SampleCreateJSPlatform->Plugin->SampleCreateJSPlugin->project->mac   (for MAC)

e. Build the project to get the DLL or .plugin (on MAC)

4.1.1 Customizing the Plugin (Optional)

If you want to customize the sample code, you will need to make following changes:

a. You can customize 'Features.xml' to enable/disable tools and menus in FlashPro.

b. Configure the parameters present in 'SampleCreateJSPlatform\Plugin\SampleCreateJSPlugin\include\PluginConfiguration.h'. This is a MANDATORY step in case you intend to re-use the plugin for your own custom platform.

4.2 Building the Eclipse project

a. Make sure you have Adobe Extension Builder plugin installed in Eclipse. You can get it from http://labs.adobe.com/technologies/extensionbuilder3/

b. Go to File -> Import. In the 'General' section of the Import dialog, select 'Existing Projects into Workspace'. Click
     on the Browse button next to the 'Select root directory' textfield and select the SamplecreateJSPlatform folder.

c. Click Finish to complete the importing of the project.

d. Right click on the project and select 'Export'. The SampleCreateJSPlatform.zxp will be created in the Eclipse workspace.

4.2.1 Customizing the Platform (Optional)

a. You can customize the publisher settings UI by updating 'index.html' and 'manifest.xml'.
b. Copy the dll/plugin file generated from step 1, into the project in 'ExtensionContent\plugin\lib\win' for Windows and 'ExtensionContent\plugin\lib\mac' for MAC.
c. Rename the *.dll files  to *.fcm files and *.plugin files to *.fcm.plugin files.
d. Copy the 'res' folder into the project to 'ExtensionContent\plugin\lib\'. 

4.3 Installation

a. Make sure you have Adobe Extension Manager CC installed. If not, please install it using the 'Adobe Creative Cloud Application'.
b. Double click on the ZXP. It will get installed using the Extension Manager.

4.4 Testing

a. Ensure that you have downloaded Mongoose web server. 
b. You must configure Mongoose using the following settings:
     listening_port = 8080
     document_root = Must be set to 'SampleCreateJSPlatform\Runtime' folder.
b. Launch FlashPro.
c. The new custom platform document type should show up in the 'Start Page' or on selecting 'File > New' menu option.
d. Create a new document of type 'SamplePlugin'. Copy all the layers from an existing AS3/Canvas/WebGL document to the new 'SamplePlugin' document. 
    If the original content contains frame-script, it will be commented out. 
f. Click on Publish Settings... button in Properties panel
g. Choose the output file path and other details in the properties panel. Output file path provided must be same as 'document_root' set in step b.
h Click Publish.
i. Verify that the output is generated in the specified output path.

Notes:
- We have tested this sample plugin only on 'Google Chrome'. Set it as the default browser for better experience.
- On selecting 'Control > Test-Movie', browser will be launched. However, on 'File -> Publish' only the output will be generated but browser will not be launched.

5. Tips for Developer:
================

a. You can control the features for the new doc type by changing 'Features.xml'. 
     For example, some of the tools like 3D tool, variable Width tool etc can be enabled/disabled. 
     However, changes in Features.xml will be reflected only on re-launching Flash.

b. Developers MUST modify ALL the configuration parameters in 'SampleCreateJSPlatform\Plugin\SampleCreateJSPlugin\include\PluginConfiguration.h'. 
    This is necessary to avoid any two plugins clashing with each other.

c. Follow the below steps to test any change made to the plugin:

  - Compile the *.dll/*.plugin in Visual Studio/XCode.
  - Package the *.zxp in eclipse
  - Install the *.zxp in extension manager
  - Open FlashPro to test

    This is needed as FlashPro will normally not load a CustomPlatform unless it is cryptographically signed. 

    However, for faster development cycle, developer can turn on debug mode in following way to bypass the signature verification.

    On Windows, open the registry key HKEY_CURRENT_USER/Software/Adobe/CSXS.5 and add a key named PlayerDebugMode, of type DWORD, and value 1.
    On Mac, open the file ~/Library/Preferences/com.adobe.CSXS.5.plist and add a row with key PlayerDebugMode, of type String, and value 1.

    Once a developer performs the above steps, he/she can quickly test the changes by doing following steps:
       - Compile the *.dll/*.plugin in Visual Studio/XCode.
       - Replace the new *.dll/*.plugin in the installed location of the zxp.

6. ChangeLogs:
=============

This section lists the changes that were done for the SDK version 1.1.0.0

Features:
=========
a. Added support for masking support in IFrameCommandGenerator service.
   The new interface ITimelineBuilder2 that is inherited from the existing ITimelineBuilder interface contains a 
   function "UpdateMask" that the plugin has to implement to make this feature work. Our sample runtime does not support 
   masking so we have commented out the body of the function UpdateMask in out sample plugin code.

b. The SampleCreateJSPlugin Xcode project now builds on Xcode 6.1   
   
Bugs Fixed:
===========
#3841079: For SWF based exporters the functionality is swapped in case of "LZMA" and "Deflate" options.
#3841085: Color transform does not get updated properly for color tweens.



