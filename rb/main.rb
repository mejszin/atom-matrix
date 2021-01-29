# Require gems
require 'rubygems'
require 'bundler'
Bundler.setup(:default, :ci)

# Require codebase
require './firebase.rb'
require './matrix.rb'
require './patterns.rb'

VERBOSE = true

# Create color matrix
cm = ColorMatrix.new
cm.heart

# Create Firebase controller
FB_KEY_PATH    = "./key.json"
FB_AUTH_DOMAIN = "https://atom-matrix-default-rtdb.europe-west1.firebasedatabase.app"
FB_PROJECT_ID  = "atom-matrix"

params = [FB_PROJECT_ID, FB_AUTH_DOMAIN, FB_KEY_PATH]

fb = FirebaseController.new(*params)

# Push color matrix data to Firebase
fb.set_to_realtime("leds", cm.data)