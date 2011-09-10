require File.dirname(__FILE__) + '/spec_helper'

describe QuadTree do

  describe "when initialized" do

    before :each do
      @quadtree = QuadTree.new(250.0, 0, 250.0, 0.0)
    end

    it "should not be nil and have it's accessible attributes " do
      @quadtree.should_not be_nil
      lambda { @quadtree.north }.should_not raise_error
      lambda { @quadtree.south }.should_not raise_error
      lambda { @quadtree.east }.should_not raise_error
      lambda { @quadtree.west }.should_not raise_error

      @quadtree.south_west.should be_nil
      @quadtree.south_east.should be_nil
      @quadtree.north_west.should be_nil
      @quadtree.north_east.should be_nil

    end

  end

  describe 'when loaded' do

    before :each do
      @quadtree = QuadTree.new(250.0, 0, 250.0, 0.0)
      # not the best dataset for this algorithm but works for testing
      10000.times do
        args = [rand*250, rand*250, rand*250]
        @quadtree.insert(*args)
      end
    end

    it "should be able to accept points and not blow up" do
      # should have been broken at least once
      quadrant = @quadtree.south_west || @quadtree.south_east || @quadtree.north_west || @quadtree.north_east
      quadrant.should_not be_nil
    end

    it 'should do proper bounding box search' do
      points = @quadtree.points_within(100, 50, 100, 50)

      # good enough test for now
      points.should_not be_empty
    end

  end


end






