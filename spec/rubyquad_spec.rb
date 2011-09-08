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

    end

    it "should be able to accept points and not blow up" do
      10000.times do
        args = [rand*250, rand*250, rand*250]
        @quadtree.insert(*args)
      end
    end

  end


end






