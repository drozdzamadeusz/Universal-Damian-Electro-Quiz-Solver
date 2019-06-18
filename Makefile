compile:

OUTPUT_PROGRAM_NAME = main

MBEDTLS = $(ROOT_DIR)/mbedtls

CFLAGS = -fPIC -DHAVE_CONFIG_H -D_U_="__attribute__((unused))" -O2

CFFLGS_SEK = `pkg-config --cflags --libs webkitgtk-3.0 libnotify`

LDFLAGS = -lm -lmenu -lncurses

INCLUDES = -I$(MBEDTLS)/include

LIBS = -lpcre2-8 libmemd.a mbedtls/library/libmbedx509.a mbedtls/library/libmbedtls.a mbedtls/library/libmbedcrypto.a

SOURCES = -Imbedtls/include main.c MainMenu.c AddTaskWindow.c Task.c TaskVariable.c TextProcessing.c JsonHelper.c MathEquationsParser.c TaskSolver.c ApiService.c preg_replace.c https.c Memd/mem.c tinyexpr.c cJSON.c

OBJS = $(SOURCES:.c=.o)


compile:
	gcc -o $(OUTPUT_PROGRAM_NAME) $(CFLAGS) $(SOURCES) $(CFFLGS_SEK) $(LIBS) $(LDFLAGS)


clear:
debug: