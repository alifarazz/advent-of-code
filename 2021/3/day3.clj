(defn get-binary-repr [s]
  (def ord-0 (int \0))
  (->> (into-array s)
       (map #(- (int %) ord-0))))

(defn v []
  (->> (slurp *in*)
       (clojure.string/split-lines)
       (mapv get-binary-repr)))

(defn seq-of-sth-to-bin-int
  ([sq] (seq-of-sth-to-bin-int sq identity))
  ([sq f]
   (defn update-value [ans bit]
     (bit-or (bit-shift-left ans 1) (f bit)))
   (reduce update-value 0 sq)))

(defn p1 [v]
  (defn- bool-to-int [x] (compare x false))
  (as->
    (reduce #(map + %1 %2) v)   xs
    (map #(> (/ (count v) 2) %) xs)
    (* (seq-of-sth-to-bin-int xs bool-to-int)
       (seq-of-sth-to-bin-int (map not xs) bool-to-int))))

(defn get-rating [v pred]
  (defn get-correct-rate [[x y]]
    (if (pred (last x) (last y)) y x))
  (loop [v   v
         idx 0]
    (if (= 2 (count v))
      (get-correct-rate v)
      (let [{ones 1 zeros 0} (group-by #(nth % idx) v)]
        (recur (if (pred (count zeros) (count ones))
                 ones
                 zeros)
               (inc idx))))))

(defn p2 [v]
  (->> [(get-rating v >) (get-rating v <)]
       (map seq-of-sth-to-bin-int)
       (reduce *)))

(time (let [v   (v)
            p1  (p1 v)
            p2  (p2 v)]
        (println p1 p2)))
