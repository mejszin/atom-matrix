module Color
    White = [128, 128, 128]
    Red = [128, 0, 0]
    Green = [0, 128, 0]
    Blue = [0, 0, 128]
end

DEFAULT_COLOR = Color::White

class ColorMatrix
    attr_accessor :data

    def initialize(w = 5, h = 5)
        @count = w * h
        @data = {}
        fill(DEFAULT_COLOR)
    end

    def fill(color)
        (0...@count).each { |index| set(index, color) }
    end

    def set(index, color)
        @data["LED_#{index}"] = {
            "R" => color[0],
            "G" => color[1],
            "B" => color[2],
        }
    end
end