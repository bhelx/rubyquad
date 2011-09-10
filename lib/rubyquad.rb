require File.dirname(__FILE__) + '/../ext/rubyquad'

class QuadTree
  attr_accessor :north, :south, :east, :west

  def points_within(north, south, east, west)
    flat = self.points_within_flat(north, south, east, west)
    return nil unless flat.any?

    points = []
    i = 0
    while i < flat.length
      points.push flat[i, i+3]
      i += 3
    end

    points
  end

end

# wanted to implement a c struct for this, maybe later
class Point
  attr_accessor :x, :y, :z
end
