#include "esphome.h"

class XboxTextSensor : public PollingComponent, public TextSensor
{
public:
  // constructor
  XboxTextSensor() : PollingComponent(15000) {}

  void setup() override
  {
    // This will be called by App.setup()
  }
  void update() override
  {
    // This will be called every "update_interval" milliseconds.
    // Publish state
    publish_state("Audio Xbox");
  }
};