require File.dirname(__FILE__) + '/spec_helper'

describe "bleah" do

  before :each do
    @quadtree = QuadTree.new(0.0, 250.0, 0.0, 250.0)
  end

  it "should do something" do
    @quadtree.should_not == nil
    lambda { @quadtree.north }.should_not raise_error
    
    10000.times do
      @quadtree.insert(rand(100), rand(100), rand(100))
    end
  end

end

