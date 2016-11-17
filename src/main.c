#include <pebble.h>

static int s_battery_level;
static Window *s_main_window;
static TextLayer *s_time_layer, *s_date_layer, *s_weather_layer;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;
static Layer *s_battery_layer;
static char *con;
static BitmapLayer *s_bt_icon_layer;
static GBitmap  *s_bt_icon_bitmap;


static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {

  Tuple *temp_t = dict_find(iter, MESSAGE_KEY_TempCond);
  if(temp_t) {
    bool second_ticks = temp_t->value->int32 == 1;
    persist_write_bool(MESSAGE_KEY_TempCond, second_ticks);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Settings Recieved as VALUE:  %d", second_ticks);
  }
}


static void prv_init(void) {
  // ...

  // Open AppMessage connection
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(128, 128);

  // ...
}




//Weather Code ------->>>
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {

  static char temperature_buffer[8];
  static char conditions_buffer[32];
  static char weather_layer_buffer[32];

  // Read tuples for data
  Tuple *temp_tuple = dict_find(iterator, MESSAGE_KEY_TEMPERATURE);
  Tuple *conditions_tuple = dict_find(iterator, MESSAGE_KEY_CONDITIONS);
  con = conditions_tuple->value->cstring;
  
  // if (strcmp(weather_units_conf, "F") == 0){
     
      //  APP_LOG(APP_LOG_LEVEL_DEBUG, "Chnaging Weather to F  %s", weather_units_conf);
  // }else{
                //     APP_LOG(APP_LOG_LEVEL_DEBUG, "Chnaging Weather to C  %s", weather_units_conf);

//   }

  
  // If all data is available, use it
  if(temp_tuple && conditions_tuple) {
   
    //int fran = (temp_tuple->value->int32*1.8)+32;
     // snprintf(temperature_buffer, sizeof(temperature_buffer), "%dF", fran);
    snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", conditions_tuple->value->cstring);
      
     // APP_LOG(APP_LOG_LEVEL_DEBUG, "Chnaging Weather to F  %s", weather_units_conf);
    //snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s", temperature_buffer);
     // text_layer_set_text(s_weather_layer, weather_layer_buffer);
   
    snprintf(temperature_buffer, sizeof(temperature_buffer), "%d", (int)temp_tuple->value->int32);
    snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", conditions_tuple->value->cstring);
      

      // Assemble full string and display
    snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s", temperature_buffer);
    text_layer_set_text(s_weather_layer, weather_layer_buffer);
    
  }
  
  //Compare Current conditions to set right background
  if (strcmp(con, "Rain") == 0){
    gbitmap_destroy(s_background_bitmap);
    s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_RAIN);
    bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
    APP_LOG(APP_LOG_LEVEL_ERROR, "Rain background Set");
    text_layer_set_text_color(s_time_layer, GColorWhite);
    text_layer_set_text_color(s_date_layer, GColorWhite);
    text_layer_set_text_color(s_weather_layer, GColorWhite);
  } else if(strcmp(con, "Clear") == 0){
    gbitmap_destroy(s_background_bitmap);
    s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_CLEAR);
    bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
    APP_LOG(APP_LOG_LEVEL_ERROR, "Clear background Set");
    text_layer_set_text_color(s_time_layer, GColorBlack);
    text_layer_set_text_color(s_date_layer, GColorBlack);
    text_layer_set_text_color(s_weather_layer, GColorBlack);
 
  } else if(strcmp(con, "Snow") == 0){
    gbitmap_destroy(s_background_bitmap);
    s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SNOW);
    bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
    APP_LOG(APP_LOG_LEVEL_ERROR, "Snow background Set");
    text_layer_set_text_color(s_time_layer, GColorBlack);
    text_layer_set_text_color(s_date_layer, GColorBlack);
    text_layer_set_text_color(s_weather_layer, GColorBlack);
    
  } else if(strcmp(con, "Mist") == 0){
    gbitmap_destroy(s_background_bitmap);
    s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_MIST);
    bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
    APP_LOG(APP_LOG_LEVEL_ERROR, "Mist background Set");
    text_layer_set_text_color(s_time_layer, GColorBlack);
    text_layer_set_text_color(s_date_layer, GColorBlack);
    text_layer_set_text_color(s_weather_layer, GColorBlack);
    
  } else if(strcmp(con, "Thunderstorm") == 0 || strcmp(con, "Thunder") == 0){
    gbitmap_destroy(s_background_bitmap);
    s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_THUNDER);
    bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
    APP_LOG(APP_LOG_LEVEL_ERROR, "Thunder background Set");
    text_layer_set_text_color(s_time_layer, GColorBlack);
    text_layer_set_text_color(s_date_layer, GColorBlack);
    text_layer_set_text_color(s_weather_layer, GColorBlack);
    
  }else if(strcmp(con, "Clouds") == 0){
    gbitmap_destroy(s_background_bitmap);
    s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_CLOUDS);
    bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
    APP_LOG(APP_LOG_LEVEL_ERROR, "Cloud background Set");
    text_layer_set_text_color(s_time_layer, GColorBlack);
    text_layer_set_text_color(s_date_layer, GColorBlack);
    text_layer_set_text_color(s_weather_layer, GColorBlack);
    
  }else{
    //Set basic background if conditions do not match
    gbitmap_destroy(s_background_bitmap);
    s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_AS);
    bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
    text_layer_set_text_color(s_time_layer, GColorBlack);
    text_layer_set_text_color(s_date_layer, GColorBlack);
    text_layer_set_text_color(s_weather_layer, GColorBlack);
  }
  
  
}
static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}


static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
   "%H:%M" : "%I:%M", tick_time);
  
    // Copy date into buffer from tm structure
  static char date_buffer[16];
  strftime(date_buffer, sizeof(date_buffer), "%a %d %b", tick_time);
  
  // Show the date
  text_layer_set_text(s_date_layer, date_buffer);
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  
  update_time();
  
  //Update weather every hour 
  if(tick_time->tm_min % 60 == 0) {
    // Begin dictionary
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);

    // Add a key-value pair
    dict_write_uint8(iter, 0, 0);

    // Send the message!
    app_message_outbox_send();
  }

}


static void bluetooth_callback(bool connected) {
  // Show icon if disconnected
  layer_set_hidden(bitmap_layer_get_layer(s_bt_icon_layer), connected);

  if(!connected) {
    // Issue a vibrating alert
    vibes_double_pulse();
  }
}

static void battery_callback(BatteryChargeState state) {
  // Record the new battery level
  s_battery_level = state.charge_percent;
  
  // Update meter
  layer_mark_dirty(s_battery_layer);
}

static void battery_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);

  // Find the width of the bar
  int width = (int)(float)(((float)s_battery_level / 100.0F) * 114.0F);

  // Draw the background
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, bounds, 0, GCornerNone);

  // Draw the bar
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, GRect(0, 0, width, bounds.size.h), 0, GCornerNone);
}

static void main_window_load(Window *window) {
 // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
// Set background to basic colour before finding weather 
  GRect bounds = layer_get_bounds(window_layer);
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_AS);
  s_background_layer = bitmap_layer_create(bounds);
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_AS);
  s_background_layer = bitmap_layer_create(bounds);
// Set the bitmap onto the layer and add to the window
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));
  
//If pebble is round set time to right postion ---- TIME
  PBL_IF_ROUND_ELSE(s_time_layer = text_layer_create(GRect(0, 50, 180, 98)), s_time_layer = text_layer_create(GRect(0, 50, 144, 98)));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  
// Settting up battery layer ----- BATTERY 
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  // Create battery meter Layer
  PBL_IF_ROUND_ELSE(s_battery_layer = layer_create(GRect(40, 25, 100, 3)), s_battery_layer = layer_create(GRect(15, 0, 115, 3)));
  layer_set_update_proc(s_battery_layer, battery_update_proc);
  layer_add_child(window_get_root_layer(window), s_battery_layer);
  
  
// Setting up date layer ---- DATE DISPLAY 
  layer_add_child(window_layer, s_battery_layer);
  PBL_IF_ROUND_ELSE(s_date_layer = text_layer_create(GRect(0, 40, 180, 75)), s_date_layer = text_layer_create(GRect(0, 30, 144, 75)));
  text_layer_set_text_color(s_date_layer, GColorWhite);
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
  
// Set up the weather layer for temp only ----- WEATHER 
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
  PBL_IF_ROUND_ELSE(s_weather_layer = text_layer_create(GRect(0, 110, 180, 75)), s_weather_layer = text_layer_create(GRect(0, 120, 144, 75)));
  text_layer_set_background_color(s_weather_layer, GColorClear);
  text_layer_set_font(s_weather_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_color(s_weather_layer, GColorWhite);
  PBL_IF_ROUND_ELSE(text_layer_set_text_alignment(s_weather_layer, GTextAlignmentCenter), text_layer_set_text_alignment(s_weather_layer, GTextAlignmentCenter));
// Add default text if weather failts 
  text_layer_set_text(s_weather_layer, "...");
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather_layer));
  
// Set up disconnect bluetooth icon ---- BLUETOOTH ICON
    s_bt_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BT_ICON);
    PBL_IF_ROUND_ELSE( s_bt_icon_layer = bitmap_layer_create(GRect(20, 40, 30, 25)),  s_bt_icon_layer = bitmap_layer_create(GRect(0, 20, 30, 25)));
    bitmap_layer_set_bitmap(s_bt_icon_layer, s_bt_icon_bitmap);
    layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_bt_icon_layer));
    // Show the correct state of the BT connection from the start
    bluetooth_callback(connection_service_peek_pebble_app_connection());
  
}

static void main_window_unload(Window *window) {
// Destroy Layers
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  layer_destroy(s_battery_layer);
  text_layer_destroy(s_weather_layer);
  gbitmap_destroy(s_background_bitmap);
  bitmap_layer_destroy(s_background_layer);
  gbitmap_destroy(s_bt_icon_bitmap);
  bitmap_layer_destroy(s_bt_icon_layer);

}




static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  // Make sure the time is displayed from the start
  update_time();
  window_set_background_color(s_main_window, GColorBlack);
  // Register for battery level updates
  battery_state_service_subscribe(battery_callback);
  // Ensure battery level is displayed from the start
  battery_callback(battery_state_service_peek());
  
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  // Register for Bluetooth connection updates
  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = bluetooth_callback
  });
  prv_init();
}




static void deinit() {
 // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}