# Change Log #

## nom::SpriteSheet Sheets ##

## v0.2.0 ##

* nomlib no longer uses json_spirit for its JSON input and output; we now use the jsoncpp library.

* Comments -- // and /* */ styles -- are not allowed anymore; this was a decision I made to ensure a greater level of compatibility with JSON parsers. Use _comment fields.

* Sorting order is alphabetical now; This is believed to be due to jsoncpp using std::map in its implementation.

* The ID field is now all lowercase: id.

## v0.1.0 ##

* Initial release
