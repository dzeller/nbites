//
//  nbfuncs.cpp
//  nbcross
//
//  Created by Philip Koch on 11/28/14.
//  Copyright (c) 2014 pkoch. All rights reserved.
//

#include "nbfuncs.h"
#include <assert.h>
#include <vector>
#include <iostream>
#include <math.h>
#include <string>

#include "Images.h"
#include "image/ImageConverterModule.cpp"
#include "RoboGrams.h"

std::vector<nbfunc_t> FUNCS;

std::vector<nbopaquelog_t> args;
std::vector<nbopaquelog_t> rets;

//Common arg types -- used to check arg types and for human readability.
char sYUVImage[] = "YUVImage";
char sParticleSwarm_pbuf[] = "ParticleSwarm";
char sParticle_pbuf[] = "Particle";
char sTest[] = "test";

char stext[] = "text";//No current sources for this data type.

int test_func() {
    for (int i = 0; i < args.size(); ++i) {
        printf("test_func(): %s\n", args[i].desc);
        rets.push_back(args[i]);
    }

    return 0;
}

#define NUM_ARG_FOR_ARG_TEST_FUNC 5
int arg_test_func() {
    printf("arg_test_func()\n");
    assert(NUM_ARG_FOR_ARG_TEST_FUNC == args.size());
    for (int i = 0; i < NUM_ARG_FOR_ARG_TEST_FUNC; ++i) {
        printf("\t%s\n", args[i].desc);
    }

    return 0;
}

int imageConverterFunc() {
    printf("image converter()\n");
    assert(args.size() == 1);

    for(int i=0; i<args.size(); i++){
        std::cout << args[i].desc << std::endl;
    }

    int width, height;

    sscanf(args[0].desc, "width=%d height=%d", &width, &height);

    messages::YUVImage image(args[0].data, width, height, width);
    portals::Message<messages::YUVImage> message(&image);
    man::image::ImageConverterModule module;

    module.imageIn.setMessage(message);
    module.run();

    const messages::PackedImage<short unsigned int>* yImage = module.yImage.getMessage(true).get();
    const messages::PackedImage<short unsigned int>* uImage = module.uImage.getMessage(true).get();
    const messages::PackedImage<short unsigned int>* vImage = module.vImage.getMessage(true).get();

    nbopaquelog_t y;

    int digits = log10((double)yImage->width()) + log10((double)yImage->height()) + 2;
    std::string tmpName;
    tmpName.append("type=YUVImage encoding=[Y16] width=");
    tmpName += yImage->width();
    tmpName += " height=";
    tmpName += yImage->height();

    char* name = new char[tmpName.size() + 1];
    memcpy(name, tmpName.c_str(), tmpName.size() + 1);

    y.desc = name;
    y.data_len = yImage->width() * yImage->height() * 2;
    y.data = new uint8_t[yImage->width() * yImage->height() * 2];
    memcpy(y.data, yImage->pixelAddress(0, 0), yImage->width() * yImage->height() * 2);

    rets.push_back(y);
    return 0;
}

void register_funcs() {
    
    /*test func (no args)*/
    nbfunc_t test;
    test.name = (char *) "test";
    test.num_args = 2;
    test.arg_names = (char **) malloc(sizeof(char *) * 2);;
    test.arg_names[0] = sTest;
    test.arg_names[1] = sTest;
    test.func = test_func;
    FUNCS.push_back(test);
    
    nbfunc_t arg_test;
    arg_test.name = (char *) "arg_test";
    arg_test.num_args = NUM_ARG_FOR_ARG_TEST_FUNC;
    arg_test.arg_names = (char **) malloc(sizeof(char *) * NUM_ARG_FOR_ARG_TEST_FUNC);
    arg_test.arg_names[0] = sYUVImage;
    arg_test.arg_names[1] = sYUVImage;
    arg_test.arg_names[2] = sParticleSwarm_pbuf;
    arg_test.arg_names[3] = sParticleSwarm_pbuf;
    arg_test.arg_names[4] = sParticleSwarm_pbuf;
    arg_test.func = arg_test_func;
    FUNCS.push_back(arg_test);

    nbfunc_t image_converter;
    image_converter.name = (char *) "image converter";
    image_converter.num_args = 1;
    image_converter.arg_names = (char **) malloc(sizeof(char *) * 1);
    image_converter.arg_names[0] = sYUVImage;
    image_converter.func = imageConverterFunc;
    FUNCS.push_back(image_converter);
}





