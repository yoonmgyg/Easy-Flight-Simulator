#include "geocoder.h"

#include <cstdlib>
#include <string>
#include <iostream>

#include <cpr/cpr.h>

Geocoder::Geocoder() {
	if(const char* env_key = std::getenv("OPENCAGE_API_KEY")) {
		api_key = (std::string) env_key;
	} else {
		throw std::runtime_error("could not load API key");
	}
}

Geocoder::Geocoder(std::string key) {
	api_key = key;
}

std::string Geocoder::geocode(std::string query) {
	std::string base_url = "https://api.opencagedata.com/geocode/v1/";
	base_url += get_format_string(); // append data format, e.g. json

	// Default params.
	auto params = cpr::Parameters{
					{"q", query},
					{"key", api_key},
					{"no_annotations", std::to_string(no_annotations)},
					{"abbrv", std::to_string(abbrv)},
					{"no_dedupe", std::to_string(no_dedupe)},
					{"no_record", std::to_string(no_record)},
					{"pretty", std::to_string(pretty)}
					};

	// Non-default params.
	if(min_confidence != 0) {
		params.AddParameter({"min_confidence", std::to_string(min_confidence)});
	}

	if(bounds != "") {
		params.AddParameter({"bounds", bounds});
	}

	if(countrycode != "") {
		params.AddParameter({"countrycode", countrycode});
	}

	if(language != "") {
		params.AddParameter({"language", language});
	}

	// This is the actual GET request.
	auto r = cpr::Get(cpr::Url{base_url}, params);

	// Handle different return codes.
	if(r.status_code == 400) {
		throw std::runtime_error("400: invalid query");
	} else if(r.status_code == 402) {
		try {
			std::string reset_time = r.header.at("X-RateLimit-Reset");
			throw std::runtime_error("402: quota exceeded, will reset at " + reset_time);
		} catch (std::out_of_range) {
			throw std::runtime_error("402: quota exceeded, but could not find when it will reset");
		}
	} else if(r.status_code == 403) {
		throw std::runtime_error("403: invalid or missing API key");
	} else if(r.status_code == 404) {
		throw std::runtime_error("404: invalid API endpoint");
	} else if(r.status_code == 408) {
		throw std::runtime_error("408: timeout, feel free to try again");
	} else if(r.status_code == 410) {
		throw std::runtime_error("410: request too long");
	} else if(r.status_code == 429) {
		throw std::runtime_error("429: too many requests, rate limit exceeded");
	} else if(r.status_code == 503) {
		throw std::runtime_error("503: server error");
	}

	return r.text;
}

std::string Geocoder::geocode_bounds(std::string query, std::string bounds) {
	// Geocode within some user-specified bounds for a single query.
	auto bounds_old = Geocoder::bounds;
	Geocoder::bounds = bounds;

	auto result = Geocoder::geocode(query);

	Geocoder::bounds = bounds_old;
	return result;
}

std::string Geocoder::get_format_string() {
	switch(output_format) {
		case OutputFormat::JSON:
			return "json";
			break;
		case OutputFormat::GEOJSON:
			return "geojson";
			break;
		case OutputFormat::XML:
			return "xml";
			break;
		case OutputFormat::MAP:
			return "map";
			break;
		case OutputFormat::GOOGLE_JSON:
			return "google-v3-json";
			break;
	}
}
