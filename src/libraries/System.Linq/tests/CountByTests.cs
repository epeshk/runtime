// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using Xunit;

namespace System.Linq.Tests
{
    public class CountByTests : EnumerableTests
    {
        [Fact]
        public void IndexerIntQuery()
        {
            int[]? q = { 3, 2, 0, 1, 2, 3, 3 };

            var countBy = q.CountBy(x => x);

            Assert.Equal(4, countBy.KeyCount);

            Assert.Equal(1, countBy[0]);
            Assert.Equal(1, countBy[1]);
            Assert.Equal(2, countBy[2]);
            Assert.Equal(3, countBy[3]);
        }

        [Fact]
        public void NonTrivialKeySelectorIntQuery()
        {
            int[]? q = { 3, 2, 0, 1, 2, 3, 3 };

            var countBy = q.CountBy(x => x * 2);

            Assert.Equal(4, countBy.KeyCount);

            Assert.Equal(1, countBy[0]);
            Assert.Equal(1, countBy[2]);
            Assert.Equal(2, countBy[4]);
            Assert.Equal(3, countBy[6]);
        }

        [Fact]
        public void ShouldNotCountNullWhenNullIsNotPresent()
        {
            string[]? q = { "a", "a" };

            var countBy = q.CountBy(x => x);

            Assert.False(countBy.Contains(null));
        }

        [Fact]
        public void ShouldCountNullWhenNullIsPresent()
        {
            string[]? q = { "a", null, "a", null, null };

            var countBy = q.CountBy(x => x);

            Assert.True(countBy.Contains(null));
            Assert.Equal(3, countBy[null]);
        }

        [Fact]
        public void NullSource_ThrowsArgumentNullException()
        {
            Func<int, int> keySelector = null;
            AssertExtensions.Throws<ArgumentNullException>("keySelector", () => new[]{1, 2}.CountBy(keySelector));
            AssertExtensions.Throws<ArgumentNullException>("keySelector", () => new[]{1, 2}.LongCountBy(keySelector));
        }
    }
}
