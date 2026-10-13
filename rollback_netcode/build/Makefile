CXX   ?= g++
AR    ?= ar

ROOT  := ..
FBNEO ?= ../../fbneo
GGPO  := $(ROOT)/third_party/ggpo/src

ARCHFLAG := -m32
ifdef BUILD_X64_EXE
  ARCHFLAG := -m64
endif

INC := -I$(ROOT)/core \
       -I$(GGPO)/include \
       -I$(GGPO)/lib/ggpo \
       -I$(FBNEO)/src/burn \
       -I$(FBNEO)/src/burn/devices

CXXFLAGS ?= -O2 -pipe
COMMON   := $(CXXFLAGS) $(ARCHFLAG) -std=gnu++17 -fno-strict-aliasing -fpermissive \
            -Wno-write-strings -Wno-unused-parameter -DBUILD_WIN32 $(INC)

OBJDIR := obj
LIB    := librollbackfbneo.a

OUR_SRC :=
OUR_SRC += $(ROOT)/core/state_ring.cpp
OUR_SRC += $(ROOT)/core/ggpo_bridge.cpp
OUR_SRC += $(ROOT)/core/nat_punch.cpp
OUR_SRC += $(ROOT)/core/relay.cpp
OUR_SRC += $(ROOT)/core/port_map.cpp
OUR_SRC += $(ROOT)/fbneo/fbneo_host.cpp
OUR_SRC += $(ROOT)/fbneo/ram_probe.cpp
OUR_SRC += $(ROOT)/fbneo/match_score.cpp
OUR_SRC += $(ROOT)/fbneo/ficha.cpp
OUR_SRC += $(ROOT)/fbneo/overlay.cpp
OUR_SRC += $(ROOT)/fbneo/hud.cpp
OUR_SRC += $(ROOT)/fbneo/end_notice.cpp
OUR_SRC += $(ROOT)/fbneo/training_notice.cpp
OUR_SRC += $(ROOT)/fbneo/loading_notice.cpp
OUR_SRC += $(ROOT)/fbneo/speedrun.cpp
OUR_SRC += $(ROOT)/fbneo/replay_menu.cpp
OUR_SRC += $(ROOT)/fbneo/frame_timing.cpp
OUR_SRC += $(ROOT)/fbneo/img_hash.cpp
OUR_SRC += $(ROOT)/fbneo/frame_pacer.cpp
OUR_SRC += $(ROOT)/fbneo/rom_notice.cpp
OUR_SRC += $(ROOT)/fbneo/anticheat.cpp
OUR_SRC += $(ROOT)/fbneo/chat.cpp
OUR_SRC += $(ROOT)/fbneo/input_display.cpp
OUR_SRC += $(ROOT)/fbneo/stream_files.cpp
OUR_SRC += $(ROOT)/fbneo/lang.cpp

GGPO_SRC :=
GGPO_SRC += $(GGPO)/lib/ggpo/bitvector.cpp
GGPO_SRC += $(GGPO)/lib/ggpo/game_input.cpp
GGPO_SRC += $(GGPO)/lib/ggpo/input_queue.cpp
GGPO_SRC += $(GGPO)/lib/ggpo/log.cpp
GGPO_SRC += $(GGPO)/lib/ggpo/main.cpp
GGPO_SRC += $(GGPO)/lib/ggpo/poll.cpp
GGPO_SRC += $(GGPO)/lib/ggpo/sync.cpp
GGPO_SRC += $(GGPO)/lib/ggpo/timesync.cpp
GGPO_SRC += $(GGPO)/lib/ggpo/platform_windows.cpp
GGPO_SRC += $(GGPO)/lib/ggpo/network/udp.cpp
GGPO_SRC += $(GGPO)/lib/ggpo/network/udp_proto.cpp
GGPO_SRC += $(GGPO)/lib/ggpo/backends/p2p.cpp
GGPO_SRC += $(GGPO)/lib/ggpo/backends/spectator.cpp
GGPO_SRC += $(GGPO)/lib/ggpo/backends/synctest.cpp

OUR_OBJ  := $(addprefix $(OBJDIR)/,$(notdir $(OUR_SRC:.cpp=.o)))
GGPO_OBJ := $(addprefix $(OBJDIR)/,$(notdir $(GGPO_SRC:.cpp=.o)))
OBJ      := $(OUR_OBJ) $(GGPO_OBJ)

vpath %.cpp $(ROOT)/core $(ROOT)/fbneo \
            $(GGPO)/lib/ggpo $(GGPO)/lib/ggpo/network $(GGPO)/lib/ggpo/backends

.PHONY: all clean
all: $(LIB)

$(LIB): $(OBJ)
	$(AR) rcs $@ $^
	@echo "  ->  $@   ($(words $(OBJ)) objects, $(ARCHFLAG))"

$(OUR_OBJ): $(OBJDIR)/%.o : %.cpp | $(OBJDIR)
	$(CXX) $(COMMON) -DUNICODE -D_UNICODE -c $< -o $@

$(GGPO_OBJ): $(OBJDIR)/%.o : %.cpp | $(OBJDIR)
	$(CXX) $(COMMON) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	@rm -rf $(OBJDIR) $(LIB)

COMMON += -MMD -MP
-include $(OBJ:.o=.d)
