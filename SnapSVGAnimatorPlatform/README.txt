This file contains guidelines on how to build and add support for Snap.SVG based plugin for Flash Professional CC using the 'Custom Platform Support Development Kit'.
This document provides the steps to build the plugin and the create the SnapSVGAnimatorSVG.zxp.

1. Software Requirements for 'SnapSVGAnimator':
   ===========================================
- Microsoft Windows 7 or Apple Mac OS 10.9 and later
- Microsoft Visual Studio 2012 (for Windows) or XCode 5.0.2
- Adobe Flash Professional CC 2015 (June/2015)
- Eclipse IDE for C/C++ Developers
- Adobe Extension Builder 3.0
- Adobe Extension Manager command line tool

- Google Chrome 

2. Installation steps for 'SnapSVGAnimator':
=======================================

a. Ensure that you have installed 'Adobe Flash Professional CC 2015'. You can install it using 'Adobe Creative Cloud Application'.
b. Ensure that you have installed 'Adobe Extension Manager' command line tool. 
c. Run the following command: 

Mac OS: ./Contents/MacOS/ExManCmd --install SnapSVGAnimator.zxp
Windows: ExManCmd.exe /install SnapSVGAnimator.zxp

d. Launch Flash CC 2015
   >> You will be able to see a new document type called 'SnapSVGAnimator' added to the Welcome Screen and to the New Document dialog (File>New...)

3. Package Contents :
=================

This contains an implementation of a custom platform 'SnapSVGAnimatorPlatform'. This platform can render SVG based output using Snap.SVG. 

It has following contents:

- Plugin : Contains C/C++ source along with the Visual Studio 2012 project and the XCode 5.0.2 projects for the plugin.
- EclipseProject - Contains an Eclipse project that can be used to generate the final platform ZXP. This also contains the 
  runtime for "SnapSVGAnimator" and the final package "SnapSVGAnimator.zxp" that can be installed using 'Adobe Extension Manager command line tool'.
- ReadMe.txt - This file contains information about this package.


4. Building, Installing and Running:
===========================

This section talks about the steps to build and install of the SnapSVGAnimatorPlatform. 

4.1 Building the C++ project

a. Ensure that you have Microsoft Visual Studio 2012 (for Windows) or XCode 5.0.2 (for MAC).

b. Copy the folders 'SnapSVGAnimatorPlatform' and 'CustomPlatformDevelopmentKit' to a folder on your system. Both this folders must be in the same level.

d. Open the solution file 'SnapSVGAnimatorPlugin.mp.64.sln' present inside:

SnapSVGAnimatorPlatform->Plugin->SnapSVGAnimatorPlugin->project->win  (for Windows)

(or)

Open 'SnapSVGAnimatorPlugin.mp.xcodeproj' present inside:

SnapSVGAnimatorPlatform->Plugin->SnapSVGAnimatorPlugin->project->mac   (for MAC)

e. Build the project to get the DLL or .plugin (on MAC)
   Note: If you do not find any schemes in Xcode, then 
		1. Go to Product->Scheme->Manage Schemes… 
                2. Click on “Autocreate Schemes Now”
                3. Select the desired scheme.

4.1.1 Customizing the Plugin (Optional)

If you want to customize the sample code, you will need to make following changes:

a. You can customize 'Features.xml' to enable/disable tools and menus in Flash Professional.

b. Configure the parameters present in 'SnapSVGAnimatorPlatform\Plugin\SnapSVGAnimatorPlugin\include\PluginConfiguration.h'. 
   This is a MANDATORY step in case you intend to re-use the plugin for your own custom platform.

4.2 Building the Eclipse project

a. Make sure you have Adobe Extension Builder plugin installed in Eclipse. You can get it from http://labs.adobe.com/technologies/extensionbuilder3/

b. Go to File -> Import. In the 'General' section of the Import dialog, select 'Existing Projects into Workspace'. Click
     on the Browse button next to the 'Select root directory' textfield and select the SamplecreateJSPlatform folder.

c. Click Finish to complete the importing of the project.

d. Right click on the project and select 'Export'. The SnapSVGAnimatorPlatform.zxp will be created in the Eclipse workspace.

4.2.1 Customizing the Platform (Optional)

a. You can customize the publisher settings UI by updating 'index.html' and 'manifest.xml'.
b. Copy the dll/plugin file generated from step 1, into the project in 'ExtensionContent\plugin\lib\win' for Windows and 'ExtensionContent\plugin\lib\mac' for MAC.
c. Rename the *.dll files  to *.fcm files and *.plugin files to *.fcm.plugin files.
d. Copy the 'res' folder into the project to 'ExtensionContent\plugin\lib\'. 

4.3 Installation

a. Make sure you have the Adobe Extension Manager command line tool installed. 
b. Run the following command from command prompt:

Mac OS: ./Contents/MacOS/ExManCmd --install SnapSVGAnimator.zxp
Windows: ExManCmd.exe /install SnapSVGAnimator.zxp

4.4 Testing

a. Launch FlashPro.
b. The new custom platform document type should show up in the 'Start Page' or on selecting 'File > New' menu option.
c. Create a new document of type 'SnapSVGAnimator'. Copy all the layers from an existing AS3/Canvas/WebGL document to the new 'SnapSVGAnimator' document. 
    If the original content contains frame-script, it will be commented out. 
d. Click on Publish Settings... button in Properties panel
e. Choose the output file path and other details in the properties panel. 
f. Click Publish.
g. Verify that the output is generated in the specified output path.

Notes:
- We have tested this plugin only on 'Google Chrome'. Set it as the default browser for better experience.
- On selecting 'Control > Test-Movie', browser will be launched. However, on 'File -> Publish' only the output will be generated, the browser will not be launched.

5. Tips for Developer:
================

a. You can control the features for the new doc type by changing 'Features.xml'. 
     For example, some of the tools like 3D tool, variable Width tool etc can be enabled/disabled. 
     However, changes in Features.xml will be reflected only on re-launching Flash.

b. Developers MUST modify ALL the configuration parameters in 'SnapSVGAnimatorPlatform\Plugin\SnapSVGAnimatorPlugin\include\PluginConfiguration.h'. 
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

