#ifndef INDEX_HTML_H
#define INDEX_HTML_H

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>Soap Dispenser Setup</title>
  <style>
    :root{--color-bg:#f3f4f6;--color-text:#1f2937;--color-bg-dark:#111827;--color-text-dark:#f9fafb;--color-primary:#2563eb;--color-primary-hover:#1d4ed8;--color-border:#d1d5db;--color-border-dark:#374151;--color-input-bg:#ffffff;--color-input-bg-dark:#374151;--color-helper-text:#6b7280;--color-error-text:#ef4444;--font-sm:0.875rem;--font-base:1rem;--font-xl:1.25rem;--font-3xl:1.875rem;--radius-lg:0.5rem;--radius-md:0.375rem;--shadow:0 1px 2px rgba(0,0,0,0.05);}html{font-family:sans-serif;line-height:1.5;background-color:var(--color-bg);color:var(--color-text);}html.dark{background-color:var(--color-bg-dark);color:var(--color-text-dark);}body{margin:0;padding:0;background-color:inherit;color:inherit;}input,select,textarea{font:inherit;color:inherit;background-color:var(--color-input-bg);border:1px solid var(--color-border);border-radius:var(--radius-lg);padding:0.625rem;width:100%;box-sizing:border-box;}html.dark input,html.dark select,html.dark textarea{background-color:var(--color-input-bg-dark);border-color:var(--color-border-dark);}button{font:inherit;border:none;cursor:pointer;border-radius:var(--radius-lg);padding:0.625rem 1.25rem;background-color:var(--color-primary);color:white;transition:background-color 0.2s;}button:hover{background-color:var(--color-primary-hover);}fieldset{border:none;margin:0;padding:0;margin-bottom:1.5rem;}legend{font-size:var(--font-xl);font-weight:600;margin-bottom:0.5rem;}label{display:block;margin-bottom:0.5rem;font-weight:500;}p{margin-top:0.25rem;font-size:var(--font-sm);color:var(--color-helper-text);}html.dark p{color:#9ca3af;}.text-red-500{color:var(--color-error-text);}.hidden{display:none;}.max-w-2xl{max-width:42rem;}.mx-auto{margin-left:auto;margin-right:auto;}.py-10{padding-top:2.5rem;padding-bottom:2.5rem;}.p-6{padding:1.5rem;}.text-3xl{font-size:var(--font-3xl);font-weight:bold;}.text-center{text-align:center;}.text-sm{font-size:var(--font-sm);}.text-xl{font-size:var(--font-xl);}.rounded-lg{border-radius:var(--radius-lg);}.shadow{box-shadow:var(--shadow);}.bg-white{background-color:white;}html.dark .bg-white{background-color:#1f2937;}.flex{display:flex;}.justify-end{justify-content:flex-end;}.items-center{align-items:center;}.gap-2{gap:0.5rem;}.space-y-6 > * + *{margin-top:1.5rem;}.focus\:outline-none:focus{outline:none;}.focus\:ring-2:focus{box-shadow:0 0 0 2px var(--color-primary);}
  </style>
</head>
<body class="bg-gray-100 text-gray-800 dark:bg-gray-900 dark:text-gray-100">
  <div class="max-w-2xl mx-auto py-10">
    <div class="flex justify-end mb-4">
      <button id="toggle-theme" class="flex items-center gap-2 bg-gray-200 text-gray-800 dark:bg-gray-700 dark:text-gray-200 px-4 py-2 rounded text-sm focus:outline-none focus:ring-2 focus:ring-blue-500">
        <span id="theme-icon">🌓</span> Theme
      </button>
    </div>
    <h1 class="text-3xl font-bold mb-6 text-center text-gray-900 dark:text-gray-100">Soap Dispenser Setup</h1>
    <form id="setup-form" class="space-y-6 bg-white text-gray-900 dark:bg-gray-800 dark:text-gray-100 p-6 rounded shadow">
      <fieldset>
        <legend class="text-xl font-semibold mb-2">WiFi</legend>
        <div class="ml-2">
          <label for="wifi_ssid" class="block mb-2 text-sm font-medium">SSID</label>
          <input type="text" id="wifi_ssid" name="wifi_ssid" class="bg-white dark:bg-gray-700 border border-gray-300 dark:border-gray-600 text-sm rounded-lg block w-full p-2.5" required>
          <p class="ml-2 text-sm text-gray-500 dark:text-gray-400">Your WiFi network name</p>
          <p class="ml-2 text-sm text-red-500 hidden" id="error-wifi_ssid"></p>
        </div>
        <div class="ml-2">
          <label for="wifi_password" class="block mb-2 text-sm font-medium">Password</label>
          <input type="password" id="wifi_password" name="wifi_password" class="bg-white dark:bg-gray-700 border border-gray-300 dark:border-gray-600 text-sm rounded-lg block w-full p-2.5" required>
          <p class="ml-2 text-sm text-gray-500 dark:text-gray-400">Your WiFi password</p>
          <p class="ml-2 text-sm text-red-500 hidden" id="error-wifi_password"></p>
        </div>
      </fieldset>

      <fieldset>
        <legend class="text-xl font-semibold mb-2">Home Assistant</legend>
        <div class="ml-2">
          <label for="ha_endpoint" class="block mb-2 text-sm font-medium">Endpoint</label>
          <input type="text" id="ha_endpoint" name="ha_endpoint" class="bg-white dark:bg-gray-700 border border-gray-300 dark:border-gray-600 text-sm rounded-lg block w-full p-2.5">
          <p class="ml-2 text-sm text-gray-500 dark:text-gray-400">Path to fetch sensor data</p>
        </div>
        <div class="ml-2">
          <label for="ha_token" class="block mb-2 text-sm font-medium">Token</label>
          <input type="text" id="ha_token" name="ha_token" class="bg-white dark:bg-gray-700 border border-gray-300 dark:border-gray-600 text-sm rounded-lg block w-full p-2.5">
          <p class="ml-2 text-sm text-gray-500 dark:text-gray-400">Long-living access token</p>
        </div>
        <div class="ml-2">
          <label for="ha_ip" class="block mb-2 text-sm font-medium">Home Assistant IP</label>
          <input type="text" id="ha_ip" name="ha_ip" class="bg-white dark:bg-gray-700 border border-gray-300 dark:border-gray-600 text-sm rounded-lg block w-full p-2.5">
        </div>
        <div class="ml-2">
          <label for="refresh_interval" class="block mb-2 text-sm font-medium">Refresh Interval (hours)</label>
          <input type="number" id="refresh_interval" name="refresh_interval" value="12" class="bg-white dark:bg-gray-700 border border-gray-300 dark:border-gray-600 text-sm rounded-lg block w-full p-2.5">
        </div>
      </fieldset>

      <fieldset>
        <legend class="text-xl font-semibold mb-2">Common</legend>
        <div class="ml-2">
          <label for="sleep_timeout" class="block mb-2 text-sm font-medium">Sleep Timeout (sec)</label>
          <input type="number" id="sleep_timeout" name="sleep_timeout" value="10" class="bg-white dark:bg-gray-700 border border-gray-300 dark:border-gray-600 text-sm rounded-lg block w-full p-2.5">
        </div>
        <div class="ml-2">
          <label for="bt_sensor" class="block mb-2 text-sm font-medium">Enable Bluetooth Sensor</label>
          <input type="checkbox" id="bt_sensor" name="bt_sensor" class="w-4 h-4">
        </div>
      </fieldset>

      <button type="submit" class="text-white bg-blue-700 hover:bg-blue-800 rounded-lg px-5 py-2.5 focus:outline-none focus:ring-2 focus:ring-blue-500">Save Settings</button>
    </form>
  </div>

  <script>
    const html = document.documentElement;
    const storedTheme = localStorage.getItem('theme');
    if (storedTheme === 'dark' || (!storedTheme && window.matchMedia('(prefers-color-scheme: dark)').matches)) {
      html.classList.add('dark');
    }

    const toggle = document.getElementById('toggle-theme');
    const icon = document.getElementById('theme-icon');
    const updateIcon = () => {
      icon.textContent = html.classList.contains('dark') ? '🌙' : '☀️';
    };
    updateIcon();

    toggle.addEventListener('click', () => {
      html.classList.toggle('dark');
      localStorage.setItem('theme', html.classList.contains('dark') ? 'dark' : 'light');
      updateIcon();
    });

    document.getElementById('setup-form').addEventListener('submit', async (e) => {
      e.preventDefault();
      const data = Object.fromEntries(new FormData(e.target).entries());
      data.bt_sensor = document.getElementById('bt_sensor').checked;

      try {
        const res = await fetch('/setup', {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify(data)
        });

        const result = await res.json();
        if (!res.ok && result.errors) {
          result.errors.forEach(err => {
            const [field, message] = Object.entries(err)[0];
            const el = document.getElementById('error-' + field);
            if (el) {
              el.textContent = message;
              el.classList.remove('hidden');
            }
          });
        } else {
          alert('Settings saved!');
        }
      } catch (err) {
        alert('Error: ' + err.message);
      }
    });
  </script>
</body>
</html>
)rawliteral";

#endif // INDEX_HTML_H
