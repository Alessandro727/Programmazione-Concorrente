//
//  dispatcher.h
//  HWC2
//
//  Created by Marco Faretra on 12/12/16.
//  Copyright © 2016 Marco Faretra. All rights reserved.
//

#ifndef dispatcher_h
#define dispatcher_h

#include <stdio.h>
#include "provider_buffer.h"
#include "reader_list.h"

void* dispatch_message();
void* remove_slow_readers();

#endif /* dispatcher_h */
