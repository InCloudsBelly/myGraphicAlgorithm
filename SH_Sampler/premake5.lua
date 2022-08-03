
publicIncludedirs =
{
   "$(ProjectDir)",
   "D:/opencv/opencv/newbuild/install/include",
   "D:/opencv/opencv/newbuild/install/include/opencv",
   "D:/opencv/opencv/newbuild/install/include/opencv2",
   "$(EIGEN)",
   "$(BOOST)",
   "$(ASSIMP)/include",
   "$(STB_IMAGE)/include",
   "$(GLM)",
   "$(OPENGL)/include/",
   "../FRAME"
}

publicLibdirs =
{
   "D:/opencv/opencv/newbuild/install/x64/vc16/lib",
   "$(PARTIO)/Release",
   "$(BOOST)/x64",
   "$(ASSIMP)/lib/x64",
   "$(OPENGL)/lib/x64"
}

publicLinks =
{
   "FRAME.lib",
   "opengl32.lib",
   "glfw3-vc140.lib",
   "assimp-vc140-mt.lib",
   "kernel32.lib",
   "user32.lib",
   "gdi32.lib",	
   "winspool.lib",
   "comdlg32.lib",
   "advapi32.lib",
   "shell32.lib",
   "ole32.lib",
   "oleaut32.lib",
   "uuid.lib",
   "odbc32.lib",
   "odbccp32.lib" ,
   "opencv_aruco345d.lib",
	"opencv_bgsegm345d.lib",
	"opencv_bioinspired345d.lib",
	"opencv_core345d.lib",
	"opencv_datasets345d.lib",
	"opencv_dnn_objdetect345d.lib",
	"opencv_dnn345d.lib",
	"opencv_dpm345d.lib",
	"opencv_face345d.lib",
	"opencv_features2d345d.lib",
	"opencv_flann345d.lib",
	"opencv_fuzzy345d.lib",
	"opencv_hfs345d.lib",
	"opencv_highgui345d.lib",
	"opencv_img_hash345d.lib",
	"opencv_imgcodecs345d.lib",
	"opencv_imgproc345d.lib",
	"opencv_line_descriptor345d.lib",
	"opencv_ml345d.lib",
	"opencv_objdetect345d.lib",
	"opencv_optflow345d.lib",
	"opencv_phase_unwrapping345d.lib",
	"opencv_photo345d.lib",
	"opencv_plot345d.lib",
	"opencv_reg345d.lib",
	"opencv_rgbd345d.lib",
	"opencv_saliency345d.lib",
	"opencv_shape345d.lib",
	"opencv_stereo345d.lib",
	"opencv_stitching345d.lib",
	"opencv_structured_light345d.lib",
	"opencv_superres345d.lib",
	"opencv_surface_matching345d.lib",
	"opencv_text345d.lib",
	"opencv_tracking345d.lib",
	"opencv_video345d.lib",
	"opencv_videoio345d.lib",
	"opencv_videostab345d.lib",
	"opencv_xfeatures2d345d.lib",
	"opencv_ximgproc345d.lib",
	"opencv_xobjdetect345d.lib",
	"opencv_xphoto345d.lib"
}



  
project("SH_Sampler")          
	kind "ConsoleApp"  
	systemversion "latest"		
	language "C++"
	
	filter { "platforms:x64" }
		system "Windows"
		architecture "x86_64"
		
	files { "**.h", "**.cpp","**.glsl","**.cu","**.cuh" }
	vpaths {
		["Headers"] = "**.h",
		["Sources/*"] = {"**.cpp"},
		["Shader"] = {"**.glsl"},
		["CUDA"] = {"**.cu","**.cuh"}
	}
	debugenvs {"PATH=$(OPENGL)/bin/x64;$(ASSIMP)/bin/x64;%PATH"}
	disablewarnings { 
		"4251",
		"4305",
		"4244"
	}
		
	includedirs {publicIncludedirs}
	links {publicLinks}	  
	libdirs {publicLibdirs}	

	filter "configurations:Debug"
	defines { "DEBUG" }
	symbols "On"
	targetdir "../x64/Debug"
	objdir "x64/Debug"
	links { "glew32d.lib", "opencv_world345d.lib" }	
	libdirs { "../x64/Debug" }
	

filter "configurations:Release"
	defines { "NDEBUG" }
	optimize "On"
	targetdir "../x64/Release"
	objdir "x64/Release"
	links { "glew32.lib", "opencv_world345.lib"}	
	libdirs { "../x64/Release" }