#include <metering/LogTagManager.hpp>
#include <ctime>

void LogTag::getTimeStr(std::string& result) const {
  // convert to C-type representation
  time_t tt = Logclock::to_time_t(time);
  struct tm* timeinfo;
  timeinfo = localtime(&tt);

  // create a string buffer for the conversion
  char buffer[20];
  // format: YYYY-mm-dd_HH:MM:ss (10+1+8 chars = 20 chars incl. '\0')
  size_t cpcnt = strftime(buffer, 20, "%F_%T", timeinfo);
  if (cpcnt > 0 && cpcnt < 20) { 
    result.assign(buffer, cpcnt);
  }
  else
    throw "Buffer overflow in LogTag's time to string conversion.";
}

void LogTagManager::tag(const std::string& key) {
  // create a new tag instance
  LogTagPtr tptr(new LogTag);
  tptr->time = Logclock::now();
  tptr->tag.assign(key);
  
  // move into container
  _tags.push_back(std::move(tptr));
}


