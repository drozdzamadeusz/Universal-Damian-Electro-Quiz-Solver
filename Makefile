compile:
	gcc -o main  main.c TaskVaraible.c TextProcessing.c MathEquationsParser.c Memd/mem.c tinyexpr.c cJSON.c -lpcre2-8  `pkg-config --cflags --libs webkitgtk-3.0 libnotify` libmemd.a -lm