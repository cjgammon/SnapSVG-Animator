This file contains guidelines on how to add support for custom platforms in Flash Professional CC using the 'Custom Platform Support Development Kit'.
The sample custom platform provided as part of this package can be used as reference by developers for developing their own custom platform.

1. Software Requirements for 'SampleCreateJSPlatform':
   ===========================================
- Microsoft Windows 7 or Apple Mac OS 10.9 and later
- Microsoft Visual Studio 2012 (for Windows) or XCode 5.0.2
- Adobe Flash Professional CC 2015 (June/2015)
- Eclipse IDE for C/C++ Developers
- Adobe Extension Builder 3.0
- Adobe Extension Manager CC 7.3.2.39

- Google Chrome (This is needed by 'SampleCreateJSPlatform' to view the published content and your custom plugin might not need this)

2. Installation steps for 'SampleCreateJSPlatform':
=======================================

a. Ensure that you have installed 'Adobe Flash Professional CC 2015'. You can install it using 'Adobe Creative Cloud Application'.
b. Ensure that you have installed 'Adobe Extension Manager CC 7.3.2.39'. You can install it using 'Adobe Creative Cloud Application'.
c. Double click on the 'SampleCreateJSPlatform.zxp' and install the plugin using Adobe Extension Manager CC. 

Note: If the Extension Manager CC is unable to install the zxp or if the Flash Professional CC 2015 does not detect the plugin then rename the SampleCreateJSPlatform.zxp to SampleCreateJSPlatform.zip and extract its content to the folder 

“/Library/Application\ Support/Adobe/CEP/extensions/com.example.SampleCreateJSPlatform” on MAC

or to the folder

“C:/Program Files(x86)/Common Files/Adobe/CEP/extensions/com.example.SampleCreateJSPlatform” on Windows

d. Close Adobe Extension Manager CC
e. Launch Flash CC 2015
   >> You will be able to see a new document type called 'SamplePlugin' added to the Welcome Screen and to the New Document dialog (File>New...)

3. Package Contents :
=================


The downloaded package contains an sample implementation of a custom platform 'SampleCreateJSPlatform'. 
This sample platform outputs to CreateJS. This sample can referred by developers to add support for custom platforms of their choice.

It has following contents:

- Plugin : Contains C/C++ source along with the Visual Studio 2012 project and the XCode 5.0.2 projects for the plugin.
- EclipseProject - Contains an Eclipse project that can be used to generate the final platform ZXP.
- SampleCreateJSPlatform.zxp - This is the final custom platform package that can be installed using 'Adobe Extension Manager CC'.
- ReadMe.txt - This file contains information about this package.


4. Building, Installing and Running:
===========================

This section talks about the steps to build and install of the SampleCreateJSPlatform. 

4.1 Building the C++ project

a. Ensure that you have Microsoft Visual Studio 2012 (for Windows) or XCode 5.0.2 (for MAC).

b. Make sure you have downloaded ''CustomPlatformDevelopmentKit''. You can get it from 'http://help.adobe.com/flash/using/enabling-support-custom-platforms.html'

c. Copy the folders 'SampleCreateJSPlatform' and 'CustomPlatformDevelopmentKit' to a folder on your system. Both this folders must be in the same level.

d. Open the solution file 'SampleCreateJSPlugin.mp.64.sln' present inside:

SampleCreateJSPlatform->Plugin->SampleCreateJSPlugin->project->win  (for Windows)

(or)

Open 'SampleCreateJSPlugin.mp.xcodeproj' present inside:

SampleCreateJSPlatform->Plugin->SampleCreateJSPlugin->project->mac   (for MAC)

e. Build the project to get the DLL or .plugin (on MAC)
   Note: If you do not find any schemes in Xcode, then 
		1. Go to Product->Scheme->Manage Schemes… 
                2. Click on “Autocreate Schemes Now”
                3. Select the desired scheme.

4.1.1 Customizing the Plugin (Optional)

If you want to customize the sample code, you will need to make following changes:

a. You can customize 'Features.xml' to enable/disable tools and menus in Flash Professional.

b. Configure the parameters present in 'SampleCreateJSPlatform\Plugin\SampleCreateJSPlugin\include\PluginConfiguration.h'. 
   This is a MANDATORY step in case you intend to re-use the plugin for your own custom platform.

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

a. Launch FlashPro.
b. The new custom platform document type should show up in the 'Start Page' or on selecting 'File > New' menu option.
c. Create a new document of type 'SamplePlugin'. Copy all the layers from an existing AS3/Canvas/WebGL document to the new 'SamplePlugin' document. 
    If the original content contains frame-script, it will be commented out. 
d. Click on Publish Settings... button in Properties panel
e. Choose the output file path and other details in the properties panel. 
f. Click Publish.
g. Verify that the output is generated in the specified output path.

Notes:
- We have tested this sample plugin only on 'Google Chrome'. Set it as the default browser for better experience.
- On selecting 'Control > Test-Movie', browser will be launched. However, on 'File -> Publish' only the output will be generated, the browser will not be launched.

5. Tips for Developer:
================

a. You can control the features for the new doc type by changing 'Features.xml'. 
     For example, some of the tools like 3D tool, variable Width tool etc can be enabled/disabled. 
     However, changes in Features.xml will be reflected only on re-launching Flash.

b. Developers MUST modify ALL the configuration parameters in 'SampleCreateJSPlatform\Plugin\SampleCreateJSPlugin\include\PluginConfiguration.h'. 
    This is necessary to avoid two plugins clashing with each other.

c. Follow the below steps to test any change made to the plugin:

  - Compile the *.dll/*.plugin in Visual Studio/XCode.
  - Package the *.zxp in eclipse
  - Install the *.zxp in extension manager
  - Open FlashPro to test

    Normally FlashPro will not load a CustomPlatform unless it is cryptographically signed. 

    However, for faster development cycle, developer can turn on debug mode in following way to bypass the signature verification.

    On Windows, open the registry key HKEY_CURRENT_USER/Software/Adobe/CSXS.6 and add a key named PlayerDebugMode, of type DWORD, and value 1.
    On Mac, open the file ~/Library/Preferences/com.adobe.CSXS.6.plist and add a row with key PlayerDebugMode, of type String, and value 1.

    After performing the above steps, the developer can quickly test the changes by performing the following steps:
       - Compile the *.dll/*.plugin in Visual Studio/XCode.
       - Replace the new *.dll/*.plugin at the installed location of the zxp.

d.  The Sample plugin contains a sample runtime based on 'CreateJS' inside the folder "SampleRuntime". The developers can replace it with their own runtime.

6. ChangeLogs:
=============

The below section lists the changes that are made in the SDK version 1.2.0.0

Features:
=========

a. API to get the bounds of IClassicText objects: The AddClassicText function in ITimelineBuilder interface now returns an object of 
   CLASSIC_TEXT_INFO_2 (instead of the old CLASSIC_TEXT_INFO), which contains a new field "bounds" representing the bounds of the IClassicText object.

b. In older releases the button instances were treated as movie-clips by both the DOM and the IFrameCommandGenerator service. In this release we have
   added APIs to support the button instances. If a IMovieClip instance also implements the IButton interface, then it is a button instance.
   The four states of a button namely "Up", "Over", "Down", "Hit" are always mapped to the frames 0,1,2 and 3 respectively. 
   Please look at the file IButton.h to know more about the button instance.

c. ILibraryItem::GetProperties() returns an additional key "SymbolType" for symbols. The value of key can be "Button", "MovieClip" or "Graphic".

d. Integrated Mongoose web server to the sample plugin. Now, there is no need to run the Mongoose server explicitly.

e. Changed sample plugin to reuse the bitmaps used for shape fills/styles.

Note: Flash Professional 2015 has added support for Bone tool, which is disabled in Custom platform documents. The contents from IK layer content 
      gets converted to frame by frame animation when pasted into a custom platform document.
   
Bugs Fixed:
===========

#3952719 IFLADocument->GetTimelines function returns scenes that were already deleted; 
#3967177 Publishing a file when the timeline contains masked hidden layers causes a crash.
#3970314 UpdateMask command refers to objects that are not added to the ITimelineBuilder.
#3984999 Graphic instance with a tween and looping option set to "single frame" is not exported correctly.
#3986998 If the dynamic text field is empty then user is unable to get text properties as the 
         function IClassicText::GetParagraphs() returns an empty list.
         Note: For empty dynamic text, IClassicText::GetParagraphs() now returns a paragraph which contains 
               all the information such as font name, font color etc.
#3987883 Shapes part of the shape tween are not re-used.
#3982650 The published output appears black in latest Google Chrome.
#3982651 The folders "images" and "sounds" are not automatically created at the output location when the output contains image/sound.
#3978122 Sample plugin does not reuse the bitmaps used for shape fills/styles.

--------------------------------------------------------------------

The below section lists the changes that are made in the SDK version 1.1.0.0

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



