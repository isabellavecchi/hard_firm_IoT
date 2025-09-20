# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(
    SRCS 	"main.c"
    		"ledRGB.c"
    		"wifiApp.c"
    		"httpServer.c"
    		"router.c"
    		"menuApp.c"
    		"menuButton.c"
    		"displayOled.c"
			"dateTimeNTP.c"
			"i2s.c"
    INCLUDE_DIRS	"."
    EMBED_FILES	"webPage/app.css"
    			"webPage/app.js"
    			"webPage/favicon.ico"
    			"webPage/index.html"
    			"webPage/jquery-3.3.1.min.js"
)
