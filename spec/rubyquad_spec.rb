require File.dirname(__FILE__) + '/spec_helper'

describe "bleah" do

  before :each do
    @quadtree = QuadTree.new(0.0, 250.0, 0.0, 250.0)
  end

  it "should do something" do
    @quadtree.should_not == nil
  end

end

