
WALI_DIR=$(PWD)/../../..
LIBWALI_DIR=$(WALI_DIR)/lib64

example: example.cpp $(LIBWALI_DIR)/libwali.so
	g++ -o $@ $< -g -I$(WALI_DIR)/Source -L$(LIBWALI_DIR) -lwali -Wl,-rpath,$(LIBWALI_DIR)
