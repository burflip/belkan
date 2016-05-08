################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../include/algebra3.cpp \
../include/arcball.cpp \
../include/glui.cpp \
../include/glui_add_controls.cpp \
../include/glui_bitmap_img_data.cpp \
../include/glui_bitmaps.cpp \
../include/glui_button.cpp \
../include/glui_checkbox.cpp \
../include/glui_column.cpp \
../include/glui_commandline.cpp \
../include/glui_control.cpp \
../include/glui_edittext.cpp \
../include/glui_filebrowser.cpp \
../include/glui_list.cpp \
../include/glui_listbox.cpp \
../include/glui_mouse_iaction.cpp \
../include/glui_node.cpp \
../include/glui_panel.cpp \
../include/glui_radio.cpp \
../include/glui_rollout.cpp \
../include/glui_rotation.cpp \
../include/glui_scrollbar.cpp \
../include/glui_separator.cpp \
../include/glui_spinner.cpp \
../include/glui_statictext.cpp \
../include/glui_string.cpp \
../include/glui_textbox.cpp \
../include/glui_translation.cpp \
../include/glui_tree.cpp \
../include/glui_treepanel.cpp \
../include/glui_window.cpp \
../include/quaternion.cpp \
../include/viewmodel.cpp 

OBJS += \
./include/algebra3.o \
./include/arcball.o \
./include/glui.o \
./include/glui_add_controls.o \
./include/glui_bitmap_img_data.o \
./include/glui_bitmaps.o \
./include/glui_button.o \
./include/glui_checkbox.o \
./include/glui_column.o \
./include/glui_commandline.o \
./include/glui_control.o \
./include/glui_edittext.o \
./include/glui_filebrowser.o \
./include/glui_list.o \
./include/glui_listbox.o \
./include/glui_mouse_iaction.o \
./include/glui_node.o \
./include/glui_panel.o \
./include/glui_radio.o \
./include/glui_rollout.o \
./include/glui_rotation.o \
./include/glui_scrollbar.o \
./include/glui_separator.o \
./include/glui_spinner.o \
./include/glui_statictext.o \
./include/glui_string.o \
./include/glui_textbox.o \
./include/glui_translation.o \
./include/glui_tree.o \
./include/glui_treepanel.o \
./include/glui_window.o \
./include/quaternion.o \
./include/viewmodel.o 

CPP_DEPS += \
./include/algebra3.d \
./include/arcball.d \
./include/glui.d \
./include/glui_add_controls.d \
./include/glui_bitmap_img_data.d \
./include/glui_bitmaps.d \
./include/glui_button.d \
./include/glui_checkbox.d \
./include/glui_column.d \
./include/glui_commandline.d \
./include/glui_control.d \
./include/glui_edittext.d \
./include/glui_filebrowser.d \
./include/glui_list.d \
./include/glui_listbox.d \
./include/glui_mouse_iaction.d \
./include/glui_node.d \
./include/glui_panel.d \
./include/glui_radio.d \
./include/glui_rollout.d \
./include/glui_rotation.d \
./include/glui_scrollbar.d \
./include/glui_separator.d \
./include/glui_spinner.d \
./include/glui_statictext.d \
./include/glui_string.d \
./include/glui_textbox.d \
./include/glui_translation.d \
./include/glui_tree.d \
./include/glui_treepanel.d \
./include/glui_window.d \
./include/quaternion.d \
./include/viewmodel.d 


# Each subdirectory must supply rules for building sources it contributes
include/%.o: ../include/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I"/home/valentin/workspace/Belkan/include" -I"/home/valentin/workspace/Belkan/jpeg_reader" -O0 -g3 -Wall -c -fmessage-length=0 -g -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


