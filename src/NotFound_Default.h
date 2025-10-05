#ifndef NOT_FOUND_PAGE_DEFAULT_H
#define NOT_FOUND_PAGE_DEFAULT_H

const char NOT_FOUND_PAGE_DEFAULT[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>404 Not Found</title>
</head>
<body>
  <h1>404 - Page Not Found</h1>
  <p>The requested page does not exist on Arduino.</p>
  <a href="/">Back to Home</a> or  <a href="https://diyables.io/stem-v4-iot">Check Tutorials</a>
</body>
</html>
)rawliteral";

#endif