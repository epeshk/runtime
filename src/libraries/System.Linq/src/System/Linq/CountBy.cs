// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Numerics;
using System.Runtime.InteropServices;
#pragma warning disable CS8714

namespace System.Linq
{
    public static partial class Enumerable
    {
        public static ICounts<TKey, int> CountBy<TSource, TKey>(this IEnumerable<TSource> source, Func<TSource, TKey> keySelector) =>
            CountBy(source, keySelector, null);

        public static ICounts<TKey, int> CountBy<TSource, TKey>(this IEnumerable<TSource> source, Func<TSource, TKey> keySelector, IEqualityComparer<TKey>? comparer) =>
            CountByInternal<TSource, TKey, int>(source, keySelector, comparer);

        public static ICounts<TKey, long> LongCountBy<TSource, TKey>(this IEnumerable<TSource> source, Func<TSource, TKey> keySelector) =>
            LongCountBy(source, keySelector, null);

        public static ICounts<TKey, long> LongCountBy<TSource, TKey>(this IEnumerable<TSource> source, Func<TSource, TKey> keySelector, IEqualityComparer<TKey>? comparer) =>
            CountByInternal<TSource, TKey, long>(source, keySelector, comparer);

        private static ICounts<TKey, TCount> CountByInternal<TSource, TKey, TCount>([NotNull] this IEnumerable<TSource>? source, [NotNull] Func<TSource, TKey>? keySelector, IEqualityComparer<TKey>? comparer) where TCount : struct, INumberBase<TCount>, IAdditionOperators<TCount, TCount, TCount>
        {
            if (source == null)
            {
                ThrowHelper.ThrowArgumentNullException(ExceptionArgument.source);
            }

            if (keySelector == null)
            {
                ThrowHelper.ThrowArgumentNullException(ExceptionArgument.keySelector);
            }

            TCount nullCount = default;
            Dictionary<TKey, TCount> countsBy = new(comparer);

            using (IEnumerator<TSource> e = source.GetEnumerator())
            {
                while (e.MoveNext())
                {
                    TKey currentKey = keySelector(e.Current);
                    if (currentKey == null)
                    {
                        checked
                        {
                            nullCount++;
                        }
                        continue;
                    }

                    ref TCount currentCount = ref CollectionsMarshal.GetValueRefOrAddDefault(countsBy, currentKey, out _);
                    checked
                    {
                        currentCount++;
                    }
                }
            }

            return new Counts<TKey, TCount>(countsBy, nullCount);
        }

        [DebuggerDisplay("KeyCount = {KeyCount}")]
        private sealed class Counts<TKey, TCount> : ICounts<TKey, TCount> where TCount : struct, INumberBase<TCount>
        {
            private readonly Dictionary<TKey, TCount> _countsBy;
            private readonly TCount _nullCount;

            public Counts(Dictionary<TKey, TCount> countsBy, TCount nullCount)
            {
                _countsBy = countsBy;
                _nullCount = nullCount;
            }

            public IEnumerator<(TKey Key, TCount Count)> GetEnumerator()
            {
                if (_nullCount != default)
                    yield return (default!, _nullCount);

                foreach ((TKey key, TCount count) in _countsBy)
                    yield return (key, count);
            }

            IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();

            public int KeyCount => _countsBy.Count + (TCount.IsZero(_nullCount) ? 0 : 1);

            public TCount this[TKey key] => key == null ? _nullCount : _countsBy[key];

            public bool Contains(TKey key) => key == null ? !TCount.IsZero(_nullCount) : _countsBy.ContainsKey(key);
        }
    }

    public interface ICounts<TKey, TCount> : IEnumerable<(TKey Key, TCount Count)>
    {
        int KeyCount { get; }

        TCount this[TKey key] { get; }

        bool Contains(TKey key);
    }
}
