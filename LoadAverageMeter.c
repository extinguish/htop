/*
htop - LoadAverageMeter.c
(C) 2004-2011 Hisham H. Muhammad
Released under the GNU GPL, see the COPYING file
in the source distribution for its full text.
*/

#include "LoadAverageMeter.h"

#include "CRT.h"
#include "Platform.h"

/*{
#include "Meter.h"
}*/

int LoadAverageMeter_attributes[] = {
   LOAD_AVERAGE_ONE, LOAD_AVERAGE_FIVE, LOAD_AVERAGE_FIFTEEN
};

int LoadMeter_attributes[] = { LOAD };

static void LoadAverageMeter_setValues(Meter* this, char* buffer, int size) {
   Platform_getLoadAverage(&this->values[0], &this->values[1], &this->values[2]);
   snprintf(buffer, size, "%.2f/%.2f/%.2f", this->values[0], this->values[1], this->values[2]);
}

static void LoadAverageMeter_display(Object* cast, RichString* out) {
   Meter* this = (Meter*)cast;
   char buffer[20];
   sprintf(buffer, "%.2f ", this->values[0]);
   RichString_write(out, CRT_colors[LOAD_AVERAGE_ONE], buffer);
   sprintf(buffer, "%.2f ", this->values[1]);
   RichString_append(out, CRT_colors[LOAD_AVERAGE_FIVE], buffer);
   sprintf(buffer, "%.2f ", this->values[2]);
   RichString_append(out, CRT_colors[LOAD_AVERAGE_FIFTEEN], buffer);
}

static void LoadMeter_setValues(Meter* this, char* buffer, int size) {
   double five, fifteen;
   Platform_getLoadAverage(&this->values[0], &five, &fifteen);
   if (this->values[0] > this->total) {
      this->total = this->values[0];
   }
   snprintf(buffer, size, "%.2f", this->values[0]);
}

static void LoadMeter_display(Object* cast, RichString* out) {
   Meter* this = (Meter*)cast;
   char buffer[20];
   sprintf(buffer, "%.2f ", ((Meter*)this)->values[0]);
   RichString_write(out, CRT_colors[LOAD], buffer);
}

MeterClass LoadAverageMeter_class = {
   .super = {
      .extends = Class(Meter),
      .delete = Meter_delete,
      .display = LoadAverageMeter_display,
   },
   .setValues = LoadAverageMeter_setValues, 
   .defaultMode = TEXT_METERMODE,
   .maxItems = 3,
   .total = 100.0,
   .attributes = LoadAverageMeter_attributes,
   .name = "LoadAverage",
   .uiName = "Load average",
   .description = "Load averages: 1 minute, 5 minutes, 15 minutes",
   .caption = "Load average: "
};

MeterClass LoadMeter_class = {
   .super = {
      .extends = Class(Meter),
      .delete = Meter_delete,
      .display = LoadMeter_display,
   },
   .setValues = LoadMeter_setValues, 
   .defaultMode = TEXT_METERMODE,
   .maxItems = 1,
   .total = 100.0,
   .attributes = LoadMeter_attributes,
   .name = "Load",
   .uiName = "Load",
   .description = "Load: average of ready processes in the last minute",
   .caption = "Load: "
};
