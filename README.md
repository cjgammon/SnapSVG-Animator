Snap SVG Platform Plugin for Flash CC
=====================================

This is a Custom Platform Plugin for Flash CC exporting animated content to SVG using the Snap.svg javascript library to render content.

This is an early stage **proof of concept**.

##Install
Open the SnapPlatform.zxp file found in the SampleCreateJSPlatform/EclipseProject directory. 
This should launch Adobe Extensions Manager. Follow instructions to install the plugin.
Copy SampleCreateJSPlatform/Runtime/js directory into project directory to load libraries necessary to run (see example). 
Testing from flash is currently hard-coded to open localhost/[project-name].html so fla and published files will need to live in localhost on your local server to test from flash. 
Published output will be in same directory as fla.

More detailed instructions can be found here:
https://helpx.adobe.com/flash/using/custom-platform-support.html

##Instructions
To read more about building or writing your own Custom Platform Plugins for Flash go here:
https://helpx.adobe.com/flash/using/enabling-support-custom-platforms.html