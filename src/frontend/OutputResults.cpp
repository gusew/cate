#include <frontend/OutputResults.hpp>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <frontend/FilesysHelper.hpp>

void OutputResults::filenameNotification(const std::string& filename, const std::string& description) const {
  // inform user about written file
  std::cout << "Wrote " << description << " to file:" << std::endl << filename << std::endl;
}

void OutputResults::_copyDependency(const std::string& filename) const {
   if (!FilesysHelper::copyFile(_relativeDir + filename, _resultsDir + filename))
    std::cout << "Failed to copy '" << filename << "' to result-directory." << std::endl;
}

void OutputResults::_htmlHeader(std::ostringstream& str) const {
  str << "<!DOCTYPE html>" << std::endl << "<html><head>" << 
    "<meta charset=\"utf-8\" />" <<
    "<title>CATE - Evaluation</title>" <<
    "<link rel=\"stylesheet\" type=\"text/css\" href=\"jqplot.css\" />" <<
    "</head><body>" <<
    "<div class=\"container\"><div id=\"overview\">" <<
    "<h1>Benchmark-Evaluation</h1>" << std::endl;
}

void OutputResults::_htmlFooter(std::ostringstream& str, const std::string& id) const {
  str << "</div>" << std::endl <<
    "<!-- Include jqPlot (manually concatenated version with all required plugins) -->" <<
    "<script language=\"javascript\" type=\"text/javascript\" src=\"jqplot.js\"></script>" << std::endl <<
    "<!-- Required data and plot-configuration -->" <<
    "<script language=\"javascript\" type=\"text/javascript\" " <<
    "src=\"" << id << "_data.js\"></script>" <<
    "<script language=\"javascript\" type=\"text/javascript\" " << 
    "src=\"" << id << "_plots.js\"></script>" << std::endl <<
    "</body></html>" << std::endl;
}

void OutputResults::_htmlInfo(std::ostringstream& str, const std::string& id, const BenchmarkInfoVector& info) const {
  str << "<h2>General Information</h2>" <<
    "<table border=\"1\" cellpadding=\"5\" cellspacing=\"0\">" << std::endl <<
    "<tr><td><strong>Benchmark-ID</strong></td>" <<
    "<td>" << id << "</td></tr>" << std::endl;
  
  for (BenchmarkInfoVector::const_iterator iter(info.cbegin()); iter != info.cend(); ++iter) {

    str << "<tr><td><strong>" << iter->first << "</strong></td>" <<
		  "<td>" << iter->second << "</td></tr>"<< std::endl;
  }

  str << "</table><hr />" << std::endl;
}

void OutputResults::_htmlChronoTable(std::ostringstream& str, const std::string& id, const ChronoEvaluation& chrono) const {
  str << "<h2>Chronograph-Results</h2><h3>Table with statistics</h3>" << 
    "<table border=\"1\" cellpadding=\"5\" cellspacing=\"0\">" <<
    "<tr><td><strong>Chronograph-Name</strong></td>" << 
    "<td><strong>Mean timespan [us]</strong></td>" <<
    "<td><strong>Std. deviation [us]</strong></td>" << 
    "<td><strong>Min. value [us]</strong></td>" << 
    "<td><strong>Max. value [us]</strong></td>" << 
    "<td><strong>Median [us]</strong></td>" << 
    "<td><strong>Mean/Headers [us]</strong></td>" <<
    "<td><strong>Mean/Rules [us]</strong></td></tr>" << std::endl;

  std::vector<std::string>::const_iterator nameItr(chrono.categoryNames.cbegin());
  std::vector<ChronoStatValuePtr>::const_iterator statItr(chrono.statistics.cbegin());
  std::vector<double>::const_iterator relHdrsItr(chrono.relativeHeaders.cbegin());
  std::vector<double>::const_iterator relRulesItr(chrono.relativeRules.cbegin());

  while (nameItr != chrono.categoryNames.cend() &&
    statItr != chrono.statistics.cend() &&
    relHdrsItr != chrono.relativeHeaders.cend() &&
    relRulesItr != chrono.relativeRules.cend()) {
    // get values for each cell
    str << "<tr><td>" << *nameItr << "</td>" << 
      "<td>" << std::setprecision(3) << std::fixed << (*statItr)->mean << "</td>" <<
      "<td>" << std::setprecision(3) << std::fixed << (*statItr)->stddev << "</td>" <<
      "<td>" << (*statItr)->minimum << "</td>" <<
      "<td>" << (*statItr)->maximum << "</td>" <<
      "<td>" << (*statItr)->median << "</td>" <<
      "<td>" << std::setprecision(3) << std::fixed << *relHdrsItr << "</td>" <<
      "<td>" << std::setprecision(3) << std::fixed << *relRulesItr << "</td>" <<
      "</tr>" << std::endl;
    ++nameItr; ++statItr; ++relHdrsItr; ++relRulesItr;
  }
  str << "</table>" <<
    "<p>See the results for each benchmark run in <a href=\"" << id <<
    "_chrono.csv\">plain csv</a>.</p>" << std::endl;
}

void OutputResults::_htmlMemTables(std::ostringstream& str, const std::string& id, const MemEvaluationGroups& mem) const {
  str << "<h2>Memory-Results</h2>" <<
    "<p>All data in the tables are mean-values taken over all testruns. " <<
    "Please check the <a href=\"" << id << "_memory.csv\">plain csv-file</a> " <<
    "for all details on the statistical evaluation.</p>" << std::endl;

  // iterate over all groups
  MemEvaluationGroups::const_iterator groupItr(mem.cbegin());
  unsigned int groupIdCnt = 0;
  while (groupItr != mem.cend()) {
    // table header for allocations and general accesses
    str << "<h3>Allocation and general access (Group-ID " << 
      std::to_string(groupIdCnt) << ")</h3>" << 
      "<table border=\"1\" cellpadding=\"5\" cellspacing=\"0\"><tr>" << 
      "<td><strong>Headers</strong></td>" <<
      "<td><strong>Allocations [Bytes]</strong></td>" << 
      "<td><strong>Max. allocated [Bytes]</strong></td>" << 
      "<td><strong>Accesses [Bytes]</strong></td>" << 
      "<td><strong>Access count</strong></td>" << 
      "<td><strong>Access Mean Size [Bytes]</strong></td>" << 
      "</tr>" << std::endl;

    // iterate over allocations and general accesses 
    MemStatCategory::const_iterator allocItr((*groupItr)->allocBytes.cbegin());
    MemStatCategory::const_iterator allocMaxItr((*groupItr)->allocMaxBytes.cbegin());
    MemStatCategory::const_iterator accItr((*groupItr)->accBytes.cbegin());
    MemStatCategory::const_iterator accCntItr((*groupItr)->accCount.cbegin());
    MemStatCategory::const_iterator meanAccItr((*groupItr)->meanAccSize.cbegin());

    while (allocItr != (*groupItr)->allocBytes.cend() && 
      allocMaxItr != (*groupItr)->allocMaxBytes.cend() &&
      accItr != (*groupItr)->accBytes.cend() &&
      accCntItr != (*groupItr)->accCount.cend() &&
      meanAccItr != (*groupItr)->meanAccSize.cend()) {
      // get values for each cell
      str << "<tr><td>" << std::to_string(allocItr->first) << "</td>" << 
      "<td>" << std::setprecision(0) << std::fixed << allocItr->second->mean << "</td>" <<
      "<td>" << std::setprecision(0) << std::fixed << allocMaxItr->second->mean << "</td>" <<
      "<td>" << std::setprecision(0) << std::fixed << accItr->second->mean << "</td>" <<
      "<td>" << std::setprecision(0) << std::fixed << accCntItr->second->mean << "</td>" <<
      "<td>" << std::setprecision(2) << std::fixed << meanAccItr->second->mean << "</td>" <<
      "</tr>" << std::endl;

      ++allocItr; ++allocMaxItr; ++accItr; ++accCntItr; ++meanAccItr;
    }
    str << "</table>" << std::endl << std::endl;

    // table header for read/write accesses
    str << "<h3>Read and write access (Group-ID " << 
      std::to_string(groupIdCnt) << ")</h3>" << 
      "<table border=\"1\" cellpadding=\"5\" cellspacing=\"0\"><tr>" << 
      "<td><strong>Headers</strong></td>" <<
      "<td><strong>Read [Bytes]</strong></td>" << 
      "<td><strong>Write [Bytes]</strong></td>" << 
      "<td><strong>Read count</strong></td>" << 
      "<td><strong>Write count</strong></td>" << 
      "<td><strong>Read Mean Size [Bytes]</strong></td>" << 
      "<td><strong>Write Mean Size [Bytes]</strong></td>" << 
      "</tr>" << std::endl;

    // iterate over read/write accesses 
    MemStatCategory::const_iterator readItr((*groupItr)->accReadBytes.cbegin());
    MemStatCategory::const_iterator writeItr((*groupItr)->accWriteBytes.cbegin());
    MemStatCategory::const_iterator readCntItr((*groupItr)->accReadCount.cbegin());
    MemStatCategory::const_iterator writeCntItr((*groupItr)->accWriteCount.cbegin());
    MemStatCategory::const_iterator readMeanItr((*groupItr)->meanReadSize.cbegin());
    MemStatCategory::const_iterator writeMeanItr((*groupItr)->meanWriteSize.cbegin());

    while (readItr != (*groupItr)->accReadBytes.cend() && 
      writeItr != (*groupItr)->accWriteBytes.cend() &&
      readCntItr != (*groupItr)->accReadCount.cend() &&
      writeCntItr != (*groupItr)->accWriteCount.cend() &&
      readMeanItr != (*groupItr)->meanReadSize.cend() &&
      writeMeanItr != (*groupItr)->meanWriteSize.cend()) {
      // get values for each cell
      str << "<tr><td>" << std::to_string(readItr->first) << "</td>" << 
      "<td>" << std::setprecision(0) << std::fixed << readItr->second->mean << "</td>" <<
      "<td>" << std::setprecision(0) << std::fixed << writeItr->second->mean << "</td>" <<
      "<td>" << std::setprecision(0) << std::fixed << readCntItr->second->mean << "</td>" <<
      "<td>" << std::setprecision(0) << std::fixed << writeCntItr->second->mean << "</td>" <<
      "<td>" << std::setprecision(2) << std::fixed << readMeanItr->second->mean << "</td>" <<
      "<td>" << std::setprecision(2) << std::fixed << writeMeanItr->second->mean << "</td>" <<
      "</tr>" << std::endl;

      ++readItr; ++writeItr; ++readCntItr; ++ writeCntItr; ++readMeanItr; ++writeMeanItr;
    }
    str << "</table>" << std::endl << std::endl;

    ++groupItr; ++groupIdCnt;
  }
}

void OutputResults::_jsSetTheme(std::ostringstream& str) const {
  str << "$(document).ready(function(){" << std::endl << 
    "$.jqplot.config.enablePlugins = true;" << std::endl << 
    "var themeSettings = {" << std::endl <<
    "grid: { drawBorder: false,	shadow: false, background: 'rgba(255, 255, 255, 0.0)' }," << std::endl <<
    "seriesDefaults: { smooth: false,	shadow: false, showMarker: true, pointLabels: { show: false }	}," << std::endl <<
    "axes: { xaxis: {	pad: 1.0, tickOptions: { showGridline: false, formatString: '%d' } },	yaxis: { pad: 1.05 } }" << std::endl <<
    "};" << std::endl;
}

void OutputResults::_jsPlotFooter(std::ostringstream& str) const {
  str << "if (!$.jqplot.use_excanvas) {" <<
    "$('div.jqplot-target').each(function() {" <<
      "var outerDiv = $(document.createElement('div')); " << 
      "var header = $(document.createElement('div')); " << 
      "var div = $(document.createElement('div')); " << 
      "outerDiv.append(header); " << 
      "outerDiv.append(div); " <<
      "outerDiv.addClass('jqplot-image-container'); " <<
      "header.addClass('jqplot-image-container-header'); " <<
      "div.addClass('jqplot-image-container-content'); " <<
      "header.html('Right Click to \\\"Save Image As...\\\" '); " <<
      "var close = $(document.createElement('a')); " <<
      "close.addClass('jqplot-image-container-close'); " <<
      "close.html('Close'); close.attr('href', '#'); " <<
      "close.click(function() { $(this).parents('div.jqplot-image-container').hide(500); }); header.append(close); $(this).after(outerDiv); outerDiv.hide(); " <<
      "outerDiv = header = div = close = null; " << std::endl;
  
  str << "if (!$.jqplot._noToImageButton) { " <<
        "var btn = $(document.createElement('button')); " <<
        "btn.text('View Plot Image'); " <<
        "btn.addClass('jqplot-image-button'); " <<
        "btn.addClass('button'); " <<
        "btn.bind('click', {chart: $(this)}, function(evt) { " <<
          "var imgelem = evt.data.chart.jqplotToImageElem(); " <<
          "var div = $(this).nextAll('div.jqplot-image-container').first(); " <<
          "div.children('div.jqplot-image-container-content').empty(); " <<
          "div.children('div.jqplot-image-container-content').append(imgelem);" <<
          "div.show(500); div = null; }); " <<
        "$(this).after(btn); btn.after('<br />'); btn = null; " <<
      "} }); } });" << std::endl;
}

void OutputResults::_jsBarPlot(std::ostringstream& str, const std::string& divContainer, const std::string& dataVar, const std::string& title, const std::string& tickVar, const std::string& xlabel, const std::string& ylabel) const {
  str << "$.jqplot('" << divContainer << "', [" << dataVar << 
    "], $.extend(true, {}, themeSettings, { title: '" << title << "', " <<
    "seriesDefaults: { renderer: $.jqplot.BarRenderer, rendererOptions: { " <<
    "barWidth:20, highlightMouseOver: true } }, " <<
    "axes: { xaxis: {	ticks: " << tickVar << ", " << 
    "tickRenderer: $.jqplot.CanvasAxisTickRenderer, " <<
    "tickOptions: { angle: -30, fontFamily: 'Helvetica', fontSize: '10pt' }, " <<
    "renderer: $.jqplot.CategoryAxisRenderer, " <<
		"label: '" << xlabel << "', " <<
    "labelOptions: { fontFamily: 'Helvetica', fontSize: '12pt'	} }, " <<
    "yaxis: {	label: '" << ylabel << "',	padMin: 0, " <<
    "labelRenderer: $.jqplot.CanvasAxisLabelRenderer , " <<
    "labelOptions: { fontFamily: 'Helvetica',	fontSize: '12pt' } } } " <<
    "}));" << std::endl;
}

void OutputResults::_jsHistogramPlot(std::ostringstream& str, const std::string& divContainer, const std::string& dataVar, const std::string& title, const std::string& xlabel, const std::string& ylabel) const {
  str << "$.jqplot('" << divContainer << "', [" << dataVar << 
    "], $.extend(true, {}, themeSettings, { title: '" << title << "', " <<
    "seriesDefaults: { renderer: $.jqplot.BarRenderer, rendererOptions: { " <<
    "barWidth:4, highlightMouseOver: true } }, " <<
    "axes: { xaxis: {	renderer: $.jqplot.CategoryAxisRenderer, " <<
		"label: '" << xlabel << "', " <<
    "labelOptions: { fontFamily: 'Helvetica', fontSize: '12pt'	}, " << 
    "tickOptions: { fontSize: '0' } }, " <<
    "yaxis: {	label: '" << ylabel << "',	padMin: 0, " <<
    "labelRenderer: $.jqplot.CanvasAxisLabelRenderer , " <<
    "labelOptions: { fontFamily: 'Helvetica',	fontSize: '12pt' } } } " <<
    "}));" << std::endl;
}

void OutputResults::_jsLinePlot1(std::ostringstream& str, const std::string& divContainer, const std::string& dataVar, const std::string& title, const std::string& tickVar, const std::string& xlabel, const std::string& ylabel) const {
  str << "$.jqplot('" << divContainer << "', [" << dataVar << 
    "], $.extend(true, {}, themeSettings, { title: '" << title << "', " <<
    "axes: { xaxis: {	ticks: " << tickVar << ", " << 
    "tickRenderer: $.jqplot.CanvasAxisTickRenderer, " <<
    "tickOptions: { angle: -30, fontFamily: 'Helvetica', fontSize: '10pt' }, " <<
		"label: '" << xlabel << "', " <<
    "labelOptions: { fontFamily: 'Helvetica', fontSize: '12pt'	} }, " <<
    "yaxis: {	label: '" << ylabel << "',	padMin: 0, " <<
    "labelRenderer: $.jqplot.CanvasAxisLabelRenderer , " <<
    "labelOptions: { fontFamily: 'Helvetica',	fontSize: '12pt' } } }, " <<
    "cursor: { show: true, zoom: true }" <<
    "}));" << std::endl;
}

void OutputResults::_jsLinePlot2(std::ostringstream& str, const std::string& divContainer, const std::string& dataVar1, const std::string& dataVar2, const std::string& s1label, const std::string& s2label, const std::string& title, const std::string& tickVar, const std::string& xlabel, const std::string& ylabel) const {
  str << "$.jqplot('" << divContainer << "', [" << dataVar1 << ", " << dataVar2 << 
    "], $.extend(true, {}, themeSettings, { title: '" << title << "', " <<
    "series: [{ label: '" << s1label << "' }, { label: '" << s2label << "' }], " <<
    "axes: { xaxis: {	ticks: " << tickVar << ", " << 
    "tickRenderer: $.jqplot.CanvasAxisTickRenderer, " <<
    "tickOptions: { angle: -30, fontFamily: 'Helvetica', fontSize: '10pt' }, " <<
		"label: '" << xlabel << "', " <<
    "labelOptions: { fontFamily: 'Helvetica', fontSize: '12pt'	} }, " <<
    "yaxis: {	label: '" << ylabel << "',	padMin: 0, " <<
    "labelRenderer: $.jqplot.CanvasAxisLabelRenderer , " <<
    "labelOptions: { fontFamily: 'Helvetica',	fontSize: '12pt' } } }, " <<
    "legend: { show: true, location: 'nw', placement: 'inside' }, " <<
    "cursor: { show: true, zoom: true }" <<
    "}));" << std::endl;
}

void OutputResults::_jsArray1(std::ostringstream& str, const std::vector<std::string>& items, const std::string& name) const {
  str << "var " << name << " = [ ";
  for (std::vector<std::string>::const_iterator iter(items.cbegin()); iter != items.cend(); ++iter) {
    str << *iter;
    if (iter < (items.cend() - 1) ) str << ", "; // for correct comma output
  }
  str << " ];" << std::endl;
}

void OutputResults::_jsArray2(std::ostringstream& str, const std::vector<std::string>& items1, const std::vector<std::string>& items2, const std::string& name) const {
  str << "var " << name << " = [ ";

  std::vector<std::string>::const_iterator iter1(items1.cbegin());
  std::vector<std::string>::const_iterator iter2(items2.cbegin());
  while (iter1 != items1.cend() && iter2 != items2.cend()) {
    str << "[" << *iter1 << ", " << *iter2 << "]";
    if (iter1 < (items1.cend() - 1) && iter2 < (items2.cend() - 1) )
      str << ", "; // for correct comma output
    ++iter1; ++iter2;
  }
  str << " ];" << std::endl;
}

void OutputResults::_htmlChronoPlots(std::ostringstream& html, std::ostringstream& data, std::ostringstream& plots, const ChronoEvaluation& chrono) const {
  unsigned int divWidth = 200 + chrono.categoryNames.size() * 20; // make dependend on number

  // place plotboxes in html
  html << "<h3>Plots</h3><h4>Comparison of chronograph-categories</h4>" <<
    "<div id=\"chrono_mean\" style=\"margin-top:20px; margin-left:20px; width:" << 
    std::to_string(divWidth) << "px; height:400px;\"></div>" <<
    "<h4>Comparison of chronograph-categories with relative values</h4>" << 
    "<div id=\"chrono_rel_headers\" style=\"margin-top:20px; margin-left:20px; width:" << 
    std::to_string(divWidth) << "px; height:400px;\"></div>" <<
    "<div id=\"chrono_rel_rules\" style=\"margin-top:20px; margin-left:20px; width:" << 
    std::to_string(divWidth) << "px; height:400px;\"></div>" <<
    "<hr />" << std::endl;

  // create plot-code
  _jsBarPlot(plots, "chrono_mean", "chrono_compare", 
    "Chronograph mean-durations (total)", 
    "chrono_compare_ticks", "Group names", "Total timespan [us]");

  _jsBarPlot(plots, "chrono_rel_headers", "chrono_compare_rel_headers", 
    "Chronograph mean-durations (per header)", 
    "chrono_compare_ticks", "Group names", "Duration per header [us]");
 
  _jsBarPlot(plots, "chrono_rel_rules", "chrono_compare_rel_rules", 
    "Chronograph mean-durations (per rule)", 
    "chrono_compare_ticks", "Group names", "Duration per rule [us]");

  // create plot-data
  std::vector<std::string> arrContent; 
  // iterate over category names
  for (std::vector<std::string>::const_iterator iter(chrono.categoryNames.cbegin()); iter != chrono.categoryNames.cend(); ++iter) {
    arrContent.push_back("'" + *iter + "'");
  }
  _jsArray1(data, arrContent, "chrono_compare_ticks");

  arrContent.clear(); // iterate over mean values
  for (std::vector<ChronoStatValuePtr>::const_iterator iter(chrono.statistics.cbegin()); iter != chrono.statistics.cend(); ++iter) {
    arrContent.push_back(std::to_string((*iter)->mean));
  }
  _jsArray1(data, arrContent, "chrono_compare");

  arrContent.clear(); // iterate over relative headers
  for (std::vector<double>::const_iterator iter(chrono.relativeHeaders.cbegin()); iter != chrono.relativeHeaders.cend(); ++iter) {
    arrContent.push_back(std::to_string(*iter));
  }
  _jsArray1(data, arrContent, "chrono_compare_rel_headers");

  arrContent.clear(); // iterate over relative rules
  for (std::vector<double>::const_iterator iter(chrono.relativeRules.cbegin()); iter != chrono.relativeRules.cend(); ++iter) {
    arrContent.push_back(std::to_string(*iter));
  }
  _jsArray1(data, arrContent, "chrono_compare_rel_rules");
}

void OutputResults::_htmlMemPlots(std::ostringstream& html, std::ostringstream& data, std::ostringstream& plots, const MemEvaluationGroups& mem) const {
  unsigned int divWidth = 200; 

  // create ticks output (only once, referenced by each mem-plot)
  std::string varNameTicks = "mem_ticks";
  if (mem.size() > 0) {
    std::vector<std::string> tickItems;
    for (MemStatCategory::const_iterator iter(mem[0]->allocBytes.cbegin()); iter != mem[0]->allocBytes.cend(); ++iter) {
      tickItems.push_back(std::to_string(iter->first));
    }
    _jsArray1(data, tickItems, varNameTicks);
    tickItems.clear();

    // set plot-box width
    divWidth += mem[0]->allocBytes.size() * 20;
  } else return; // if no groups, then also no data present

  // create outputstreams for each type of plots
  std::ostringstream htmlAlloc, htmlAccess, htmlRdWr;
  htmlAlloc << "<h3>Allocation Plots</h3>" << std::endl;
  htmlAccess << "<h3>Access Plots</h3>" << std::endl;
  htmlRdWr << "<h3>Access Read-Write Plots</h3>" << std::endl;

  // iterate over groups 
  MemEvaluationGroups::const_iterator groupItr(mem.cbegin());
  unsigned int groupIdCnt = 0;
  std::vector<std::string> dataItems1, dataItems2; // for js-plot data
  while (groupItr != mem.cend()) {
    std::string groupId = std::to_string(groupIdCnt);

    // allocation plots
    std::string divAlloc = "mem_alloc_" + groupId;
    std::string varNameAlloc = "mem_alloc_" + groupId;
    std::string varNameMax = "mem_alloc_max_" + groupId;

    htmlAlloc << "<h4>Allocated and max. allocated Memory (Group-ID " << groupId << ")</h4>"<<
      "<div id=\"" << divAlloc << "\" style=\"margin-top:20px; margin-left:20px; width:" << std::to_string(divWidth) << "px; height:400px;\"></div>" << std::endl;

    _jsLinePlot2(plots, divAlloc, varNameAlloc, varNameMax,
      "allocated", "max. allocated", 
      "Memory allocation for group " + groupId, varNameTicks, 
      "Processed headers", "Allocated Bytes");
   
    for (MemStatCategory::const_iterator iter((*groupItr)->allocBytes.cbegin()); iter != (*groupItr)->allocBytes.cend(); ++iter) {
      dataItems1.push_back(std::to_string(iter->first));
      dataItems2.push_back(std::to_string(iter->second->mean));
    }
    _jsArray2(data, dataItems1, dataItems2, varNameAlloc); // js-data: allocated bytes
    dataItems1.clear();    
    dataItems2.clear();    
    
    for (MemStatCategory::const_iterator iter((*groupItr)->allocMaxBytes.cbegin()); iter != (*groupItr)->allocMaxBytes.cend(); ++iter) {
      dataItems1.push_back(std::to_string(iter->first));
      dataItems2.push_back(std::to_string(iter->second->mean));
    }
    _jsArray2(data, dataItems1, dataItems2, varNameMax); // js-data: max. allocated bytes
    dataItems1.clear();    
    dataItems2.clear();    


    // access plots
    std::string divAccess = "mem_access_" + groupId;
    std::string divAccessCnt = "mem_access_cnt_" + groupId;
    std::string varNameAccess = "mem_access_" + groupId;
    std::string varNameAccessCnt = "mem_access_cnt_" + groupId;

    htmlAccess << "<h4>Accessed Memory in Bytes (Group-ID " << groupId << ")</h4>"<<
      "<div id=\"" << divAccess << "\" style=\"margin-top:20px; margin-left:20px; width:" << std::to_string(divWidth) << "px; height:400px;\"></div>" << std::endl;

    htmlAccess << "<h4>Number of Memory accesses (Group-ID " << groupId << ")</h4>"<<
      "<div id=\"" << divAccessCnt << "\" style=\"margin-top:20px; margin-left:20px; width:" << std::to_string(divWidth) << "px; height:400px;\"></div>" << std::endl;

    _jsLinePlot1(plots, divAccess, varNameAccess,
      "Memory accesses in total for group " + groupId, varNameTicks, 
      "Processed headers", "Accessed Bytes");
   
    _jsLinePlot1(plots, divAccessCnt, varNameAccessCnt,
      "Number of memory accesses for group " + groupId, varNameTicks, 
      "Processed headers", "Number of accesses");
   
    for (MemStatCategory::const_iterator iter((*groupItr)->accBytes.cbegin()); iter != (*groupItr)->accBytes.cend(); ++iter) {
      dataItems1.push_back(std::to_string(iter->first));
      dataItems2.push_back(std::to_string(iter->second->mean));
    }
    _jsArray2(data, dataItems1, dataItems2, varNameAccess); // js-data: accessed bytes
    dataItems1.clear();    
    dataItems2.clear();    
    
    for (MemStatCategory::const_iterator iter((*groupItr)->accCount.cbegin()); iter != (*groupItr)->accCount.cend(); ++iter) {
      dataItems1.push_back(std::to_string(iter->first));
      dataItems2.push_back(std::to_string(iter->second->mean));
    }
    _jsArray2(data, dataItems1, dataItems2, varNameAccessCnt); // js-data: access count
    dataItems1.clear();    
    dataItems2.clear();    


    // read-write plots
    std::string divRwBytes = "mem_rw_bytes_" + groupId;
    std::string divRwCount = "mem_rw_count_" + groupId;
    std::string varNameReadBytes = "mem_read_bytes_" + groupId;
    std::string varNameWriteBytes = "mem_write_bytes_" + groupId;
    std::string varNameReadCount = "mem_read_count_" + groupId;
    std::string varNameWriteCount = "mem_write_count_" + groupId;

    htmlRdWr << "<h4>Read and Write Memory-Accesses in Bytes (Group-ID " << groupId << ")</h4>"<<
      "<div id=\"" << divRwBytes << "\" style=\"margin-top:20px; margin-left:20px; width:" << std::to_string(divWidth) << "px; height:400px;\"></div>" << std::endl;

    htmlRdWr << "<h4>Number of Read and Write Memory-Accesses (Group-ID " << groupId << ")</h4>"<<
      "<div id=\"" << divRwCount << "\" style=\"margin-top:20px; margin-left:20px; width:" << std::to_string(divWidth) << "px; height:400px;\"></div>" << std::endl;

    _jsLinePlot2(plots, divRwBytes, varNameReadBytes, varNameWriteBytes,
      "read", "write", 
      "Memory-Accesses (read/write) in Bytes for group " + groupId, varNameTicks, 
      "Processed headers", "Accessed Bytes");

    _jsLinePlot2(plots, divRwCount, varNameReadCount, varNameWriteCount,
      "read", "write", 
      "Number of Memory-Accesses (read/write) for group " + groupId, varNameTicks, 
      "Processed headers", "Number of Accesses");
   
    for (MemStatCategory::const_iterator iter((*groupItr)->accReadBytes.cbegin()); iter != (*groupItr)->accReadBytes.cend(); ++iter) {
      dataItems1.push_back(std::to_string(iter->first));
      dataItems2.push_back(std::to_string(iter->second->mean));
    }
    _jsArray2(data, dataItems1, dataItems2, varNameReadBytes); // js-data: read bytes
    dataItems1.clear();    
    dataItems2.clear();    
     
    for (MemStatCategory::const_iterator iter((*groupItr)->accWriteBytes.cbegin()); iter != (*groupItr)->accWriteBytes.cend(); ++iter) {
      dataItems1.push_back(std::to_string(iter->first));
      dataItems2.push_back(std::to_string(iter->second->mean));
    }
    _jsArray2(data, dataItems1, dataItems2, varNameWriteBytes); // js-data: write bytes
    dataItems1.clear();    
    dataItems2.clear();    
    
    for (MemStatCategory::const_iterator iter((*groupItr)->accReadCount.cbegin()); iter != (*groupItr)->accReadCount.cend(); ++iter) {
      dataItems1.push_back(std::to_string(iter->first));
      dataItems2.push_back(std::to_string(iter->second->mean));
    }
    _jsArray2(data, dataItems1, dataItems2, varNameReadCount); // js-data: read access count
    dataItems1.clear();    
    dataItems2.clear();    

    for (MemStatCategory::const_iterator iter((*groupItr)->accWriteCount.cbegin()); iter != (*groupItr)->accWriteCount.cend(); ++iter) {
      dataItems1.push_back(std::to_string(iter->first));
      dataItems2.push_back(std::to_string(iter->second->mean));
    }
    _jsArray2(data, dataItems1, dataItems2, varNameWriteCount); // js-data: write access count
    dataItems1.clear();    
    dataItems2.clear();    

    ++groupItr; ++groupIdCnt;
  }
  html << htmlAlloc.str() << htmlAccess.str() << htmlRdWr.str() << "<hr />" << std::endl;
}

void OutputResults::_htmlHistogram(const std::string& id, std::ostringstream& html, std::ostringstream& data, std::ostringstream& plots, const std::vector<HistogramPair>& hist) const {
  html << "<h2>Matches-Histogram</h2>" << std::endl <<
    "<p>See the matching results (rule indices for each processed header) in <a href=\"" << id << "_matches.csv\">plain csv</a>.</p>" << std::endl;
  
  // place plotbox in html
  std::string divHist = "hist_matches";
  std::string varNameHist = "hist_data";
  unsigned int divWidth = 200 + hist.size() * 4; // make dependend on number of histogram-bins
  html << "<div id=\"" << divHist << "\" style=\"margin-top:20px; margin-left:20px; width:" << std::to_string(divWidth) << "px; height:400px;\"></div>" << "<hr />" << std::endl;

  // create plot-code
  _jsHistogramPlot(plots, divHist, varNameHist, 
    "Histogram of matched headers per rule", 
    "Rule index", "number of matched headers");

  // create plot-data
  std::vector<std::string> arrData1, arrData2; 
  // iterate over category names
  for (std::vector<HistogramPair>::const_iterator iter(hist.cbegin()); iter != hist.cend(); ++iter) {
    arrData1.push_back(std::to_string(iter->first));
    arrData2.push_back(std::to_string(iter->second));
  }
  _jsArray2(data, arrData1, arrData2, varNameHist);
  arrData1.clear();
  arrData2.clear();
}

void OutputResults::_benchmarkInfo(std::ostringstream& str, const std::string& id, const BenchmarkInfoVector& info) const {
  str << "Benchmark-ID; " << id << std::endl;

  for (BenchmarkInfoVector::const_iterator iter(info.cbegin()); iter != info.cend(); ++iter) {
    str << iter->first << "; " << iter->second << std::endl;
  }
}

void OutputResults::_logTags(std::ostringstream& str, const LogTagVector& tags) const {
  for (auto iter = tags.cbegin(); iter != tags.cend(); ++iter) {
    str << *iter << std::endl;
  }
}

void OutputResults::_writeFile(std::ostringstream& str, const std::string& filename) const {
  try {
    std::ofstream outFile(filename, std::ofstream::out);
    outFile << str.str();
    outFile.close();
  } catch(std::ios_base::failure failed) {
    std::cerr << "Failed to write results to '" << filename << "'. Reason: " << failed.what() << std::endl;
  }
}

void OutputResults::_csvChronograph(std::ostringstream& str, const ChronoEvaluation& chrono) const {
  unsigned int chronoCategories = chrono.categoryNames.size();

  // sanity checks
  if (chronoCategories > chrono.measurements.size() || 
    chronoCategories > chrono.statistics.size())
    throw "The number of Chronograph category names doesn't match number of entries in evaluation data.";

  // continue, if any categories available
  if (chronoCategories > 0) {
    // determine how many testruns were done
    unsigned int chronoRuns = chrono.measurements[0]->size();
    str << "chronograph_name; ";
    for (unsigned int i = 0; i < chronoRuns; ++i) 
      str << "timespan_run" << std::to_string(i+1) << "_[us]; ";
    str << "mean[us]; stddev[us]; " << std::endl;

    // iterate over all categories
    for (unsigned int catItr = 0; catItr < chronoCategories; ++catItr) {
      str << chrono.categoryNames[catItr] << "; ";

      // iterate over all testruns
      for (unsigned int runItr = 0; runItr < chronoRuns; ++runItr) {
        str << std::to_string(chrono.measurements[catItr]->at(runItr)) << "; ";
      }

      // add mean and std-dev values
      str << std::to_string(chrono.statistics[catItr]->mean) << "; ";
      str << std::to_string(chrono.statistics[catItr]->stddev) << ";";
      str << std::endl;
    }
  }
  else 
    str << "(No chronograph results were measured.)" << std::endl;
}

void OutputResults::_csvMatches(std::ostringstream& str, const Generic::RuleIndexSet& indices) const {
  str << "header_number; index_matching_rule;" << std::endl;
  unsigned int number = 1;

  for (Generic::RuleIndexSet::const_iterator iter(indices.cbegin()); iter != indices.cend(); ++iter) {
    str << number++ << "; ";
    if (*iter < Generic::noRuleIsMatching())
      str << std::to_string(*iter) << std::endl;
    else
      str << "none" << std::endl;
  }
}

void OutputResults::_csvMemory(std::ostringstream& str, const MemEvaluationGroups& mem) const {
  // output gigantic csv-header
  str << "mem_group; headers; " <<
    "allocated_bytes_mean; allocated_bytes_stddev; " <<
    "max_allocated_bytes_mean; max_allocated_bytes_stddev; " <<
    "accessed_bytes_mean; accessed_bytes_stddev; " <<
    "accessed_read_bytes_mean; accessed_read_bytes_stddev; " <<
    "accessed_write_bytes_mean; accessed_write_bytes_stddev; " << 
    "access_count_mean; access_count_stddev; " << 
    "access_read_count_mean; access_read_count_stddev; " <<
    "access_write_count_mean; access_write_count_stddev; " <<
    "access_mean_size; access_mean_size_stddev; " <<
    "read_mean_size; read_mean_size_stddev; " <<
    "write_mean_size; write_mean_size_stddev; " << std::endl;

  // iterate over each group
  unsigned int groupCounter = 0;
  for (MemEvaluationGroups::const_iterator groupItr(mem.cbegin()); groupItr != mem.cend(); ++groupItr) {
    unsigned int numberValues = (*groupItr)->allocBytes.size();
   
    // iterate over all values in the group
    for (unsigned int valIdx = 0; valIdx < numberValues; ++valIdx) {
      str << std::to_string(groupCounter) << "; ";
      str << std::to_string((*groupItr)->allocBytes[valIdx].first) << "; ";

      str << std::setprecision(0) << std::fixed << (*groupItr)->allocBytes[valIdx].second->mean << "; ";
      str << std::setprecision(2) << std::fixed << (*groupItr)->allocBytes[valIdx].second->stddev << "; ";

      str << std::setprecision(0) << std::fixed << (*groupItr)->allocMaxBytes[valIdx].second->mean << "; ";
      str << std::setprecision(2) << std::fixed << (*groupItr)->allocMaxBytes[valIdx].second->stddev << "; ";

      str << std::setprecision(0) << std::fixed << (*groupItr)->accBytes[valIdx].second->mean << "; ";
      str << std::setprecision(2) << std::fixed << (*groupItr)->accBytes[valIdx].second->stddev << "; ";

      str << std::setprecision(0) << std::fixed << (*groupItr)->accReadBytes[valIdx].second->mean << "; ";
      str << std::setprecision(2) << std::fixed << (*groupItr)->accReadBytes[valIdx].second->stddev << "; ";

      str << std::setprecision(0) << std::fixed << (*groupItr)->accWriteBytes[valIdx].second->mean << "; ";
      str << std::setprecision(2) << std::fixed << (*groupItr)->accWriteBytes[valIdx].second->stddev << "; ";

      str << std::setprecision(0) << std::fixed << (*groupItr)->accCount[valIdx].second->mean << "; ";
      str << std::setprecision(2) << std::fixed << (*groupItr)->accCount[valIdx].second->stddev << "; ";

      str << std::setprecision(0) << std::fixed << (*groupItr)->accReadCount[valIdx].second->mean << "; ";
      str << std::setprecision(2) << std::fixed << (*groupItr)->accReadCount[valIdx].second->stddev << "; ";

      str << std::setprecision(0) << std::fixed << (*groupItr)->accWriteCount[valIdx].second->mean << "; ";
      str << std::setprecision(2) << std::fixed << (*groupItr)->accWriteCount[valIdx].second->stddev << "; ";

      str << std::setprecision(2) << std::fixed << (*groupItr)->meanAccSize[valIdx].second->mean << "; ";
      str << std::setprecision(2) << std::fixed << (*groupItr)->meanAccSize[valIdx].second->stddev << "; ";

      str << std::setprecision(2) << std::fixed << (*groupItr)->meanReadSize[valIdx].second->mean << "; ";
      str << std::setprecision(2) << std::fixed << (*groupItr)->meanReadSize[valIdx].second->stddev << "; ";

      str << std::setprecision(2) << std::fixed << (*groupItr)->meanWriteSize[valIdx].second->mean << "; ";
      str << std::setprecision(2) << std::fixed << (*groupItr)->meanWriteSize[valIdx].second->stddev << "; ";

      str << std::endl;
    }
    
    ++groupCounter; // continue with next group
  }
}

void OutputResults::headers(const Generic::PacketHeaderSet& headers) const {
  std::string filename = _resultsDir + _benchmark->id + "_headers.csv";

  try {
    std::ofstream outFile(filename, std::ofstream::out | std::ofstream::app); // append to file, if exists
    
    using namespace Generic;

    for (PacketHeaderSet::const_iterator line = headers.begin(); line != headers.end(); ++line) {
      for (PacketHeaderLine::const_iterator atom = (*line)->begin(); atom != (*line)->end(); ++atom) {
        outFile << (*atom)->value << ";";
      }
      outFile << std::endl;
    }
    
    outFile.close();
  } catch(std::ios_base::failure failed) {
    std::cerr << "Failed to write headers to '" << filename << "'. Reason: " << failed.what() << std::endl;
  }
}
void OutputResults::evaluation(const BenchmarkEvaluation& eval) const {
  std::ostringstream composeHtml; // for the html-output
  std::ostringstream composeData; // for js-data output
  std::ostringstream composePlots; // for js-jqplot configuration
  std::ostringstream composeInfo; // for general key-value info
  std::ostringstream composeLogTags; // for logged tags during benchmarks

  // generate and output html-summary with plots
  _htmlHeader(composeHtml);
  _jsSetTheme(composePlots);
  _htmlInfo(composeHtml, _benchmark->id, eval.benchmarkInfo);
  _htmlChronoTable(composeHtml, _benchmark->id, eval.chrono);
  _htmlChronoPlots(composeHtml, composeData, composePlots, eval.chrono);
  _htmlMemTables(composeHtml, _benchmark->id, eval.mem);
  _htmlMemPlots(composeHtml, composeData, composePlots, eval.mem);
  if (eval.allIndicesMatch) 
    _htmlHistogram(_benchmark->id, composeHtml, composeData, composePlots, eval.histogram);
  _htmlFooter(composeHtml, _benchmark->id);
  _jsPlotFooter(composePlots);

  // generate key-value pairs with general information on benchmark
  _benchmarkInfo(composeInfo, _benchmark->id, eval.benchmarkInfo);

  // write logged tags into stringstream
  _logTags(composeLogTags, eval.logTags);

  // build filenames for summary-files to output
  std::string filePrefix = _resultsDir + _benchmark->id; 
  std::string fileHtml = filePrefix + "_index.html";
  std::string fileData = filePrefix + "_data.js";
  std::string filePlot = filePrefix + "_plots.js";
  // and related csv-files which will be referenced in the summary
  std::string fileCsvChrono = filePrefix + "_chrono.csv"; 
  std::string fileCsvMatches = filePrefix + "_matches.csv"; 
  std::string fileCsvMem = filePrefix + "_memory.csv"; 
  // and some machine readable benchmark information
  std::string fileInfo = filePrefix + "_info.csv";
  std::string fileLogTags = filePrefix + "_logtags.csv";

  // write text content to files
  _writeFile(composeHtml, fileHtml);
  _writeFile(composeData, fileData);
  _writeFile(composePlots, filePlot);
  filenameNotification(fileHtml, "benchmark-summary");

  // copy all dependency-files to result-dir
  _copyDependency("jqplot.js");
  _copyDependency("jqplot.css");

  // output csv-files with plain results  
  std::ostringstream csvChrono;
  std::ostringstream csvMatches;
  std::ostringstream csvMemory;
  
  _csvChronograph(csvChrono, eval.chrono);
  _csvMemory(csvMemory, eval.mem);

  _writeFile(csvChrono, fileCsvChrono);
  _writeFile(csvMemory, fileCsvMem);

  // treat match-indices seperately
  if (eval.allIndicesMatch) {
    _csvMatches(csvMatches, eval.indices);
    _writeFile(csvMatches, fileCsvMatches);
  }
  else
    std::cout << "Matching indices between headers and rules were not consistent over all testruns. Please check the classification algorithm!" << std::endl;

  // output general benchmark information to file
  _writeFile(composeInfo, fileInfo);

  // output logged tags to file, if any tags were logged
  if (eval.logTags.size() > 0) {
    _writeFile(composeLogTags, fileLogTags);
  }
}

