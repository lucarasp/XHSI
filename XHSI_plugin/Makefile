CC ?= gcc
CFLAGS = -O2 -I./SDK/CHeaders/XPLM -I./SDK/CHeaders/Widgets -I./SDK/CHeaders/Wrapper -fPIC -Wall
CPPFLAGS = -DLIN=1 -DXPLM200 -DXPLM210 -DXPLM300 -DXPLM301 -DXPLM303

OUT = lin.xpl

%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

obj_files = commands.o \
	datarefs.o \
	datarefs_cl30.o \
	datarefs_ff_a320.o \
	datarefs_jar_a320neo.o \
	datarefs_pa_a320.o \
	datarefs_pilotedge.o \
	datarefs_qpac.o \
	datarefs_ufmc.o \
	datarefs_x737.o \
	datarefs_x_raas.o \
	datarefs_xjoymap.o \
	datarefs_z737.o \
	endianess.o \
	jar_a320neo_msg.o \
	net.o \
	packets.o \
	plugin.o \
	qpac_msg.o \
	receiver.o \
	sender.o \
	settings.o \
	ufmc.o \
	ui.o \
	xfmc.o \
	xp11_cdu.o \
	z737_fmc.o 

all: $(obj_files)
	$(CC) -shared -o $(OUT) $(obj_files)

clean:
	rm -rf $(obj_files) $(OUT)
