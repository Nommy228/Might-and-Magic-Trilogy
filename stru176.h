#pragma once


/*  169 */
#pragma pack(push, 1)
struct stru176
{
  __int64 next_fire_time;      // timer will either fire event at this time (type 2, field_C == 0)
  __int16 timer_evt_ID;
  __int16 timer_evt_seq_num;
  __int16 time_left_to_fire;
  __int16 fire_interval;       // or fire on these intervals (type 1)
  __int16 field_10;
  __int16 field_12;
  __int16 field_14;
  __int16 field_16;
  __int16 field_18;
  __int16 field_1A;
  __int16 timer_evt_type;
  __int16 field_1E;
};
#pragma pack(pop)
extern stru176 array_5B5928_timers[100];

