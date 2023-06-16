#include "esphome.h"

class PcTextSensor : public PollingComponent, public TextSensor
{
public:
  // constructor
  PcTextSensor() : PollingComponent(15000) {}

  void setup() override
  {
    // This will be called by App.setup()
  }
  void update() override
  {
    // This will be called every "update_interval" milliseconds.
    // Publish state
    publish_state("Audio Pc");
  }
};