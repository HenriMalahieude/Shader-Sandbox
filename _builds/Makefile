CC = g++
WARN_FLAGS = -Wall -Wextra -Wno-missing-braces -Wno-missing-field-initializers
RAYLIB_SRC ?= ../../raylib/src/
FILE_FLAGS = -I $(RAYLIB_SRC) -L $(RAYLIB_SRC) -lraylib -lopengl32 -lgdi32 -lwinmm
ALL_FLAGS = $(WARN_FLAGS) $(FILE_FLAGS) -g3 -Og

warp:
	$(CC) ../warp.cpp -o warp.exe $(ALL_FLAGS)

solar_tex:
	@$(CC) ../solar.cpp -o solar_tex.exe $(ALL_FLAGS)

basic_lighting:
	@$(CC) ../basic_lighting.cpp -o basic_lighting.exe $(ALL_FLAGS)

camglitch2:
	@$(CC) ../camglitch2.cpp -o camglitch2.exe $(ALL_FLAGS)

cam_glitch:
	@$(CC) ../camera_glitch.cpp -o cam_glitch.exe $(ALL_FLAGS)

lens_flare:
	@$(CC) ../lensflare.cpp -o lensflare.exe $(ALL_FLAGS)

lighting:
	@$(CC) ../sun.cpp -o lighting.exe $(ALL_FLAGS)

stars: ../stars_by_instancing.cpp
	@$(CC) ../stars_by_instancing.cpp -o stars.exe $(ALL_FLAGS)

instancing: ../instancing.cpp
	@$(CC) ../instancing.cpp -o instancing.exe $(ALL_FLAGS)

clean:
	-@rm -f *.o 
	-@rm -f *.exe