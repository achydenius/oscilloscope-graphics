#include <OscilloscopeGraphics.h>

osc::ArduinoRenderer renderer(10, A0, A1,
                              osc::ArduinoRenderer::DACWriteMode::INLINE);
osc::Buffer<osc::Line> lines(1000);

void parseSerialData(const char* data, osc::Buffer<osc::Line>& lines) {
  int coordCount = (int)data[0];
  float* coords = (float*)&data[4];

  lines.reset();
  for (int i = 0; i < coordCount; i += 2) {
    osc::Line line;
    line.a[0] = coords[i];
    line.a[1] = coords[i + 1];
    line.b[0] = coords[(i + 2) % 8];
    line.b[1] = coords[(i + 3) % 8];
    lines.add(line);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(0);
}

void loop() {
  if (Serial.available()) {
    String data = Serial.readString();
    parseSerialData(data.c_str(), lines);
  }

  for (int i = 0; i < 4; i++) {
    renderer.drawLine(lines[i].a, lines[i].b);
  }
}
