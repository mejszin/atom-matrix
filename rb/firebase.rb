require 'firebase'

class FirebaseController
    attr_accessor :project_id, :base_uri, :key_path
    def initialize(project_id, base_uri, key_path)
        @project_id = project_id
        @base_uri   = base_uri
        @key_path   = key_path
        @auth_token = File.open(@key_path).read
    end

    def push_to_realtime(path, datum)
        firebase = Firebase::Client.new(@base_uri, @auth_token)
        response = firebase.push(path, datum)
        puts response.success? ? "Data pushed to database" : "Data could not be pushed" if VERBOSE
        return response.success?
    end

    def set_to_realtime(path, datum)
        firebase = Firebase::Client.new(@base_uri, @auth_token)
        response = firebase.set(path, datum)
        puts response.success? ? "Data added to database" : "Data could not be added" if VERBOSE
        return response.success?
    end
end